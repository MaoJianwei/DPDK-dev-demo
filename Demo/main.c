
#include <rte_eal.h>
#include <stdbool.h>
#include <signal.h>
#include <rte_mbuf.h>
#include <rte_ethdev.h>
#include <rte_malloc.h>
#include <rte_lpm.h>

#include "portConfigure.h"
#include "portLcoreBinding.h"
#include "MaoConstant.h"





struct rte_mempool * rxMbufPool;

struct rte_eth_dev_tx_buffer * txBuffers[Mao_MAX_ETHPORTS];

struct PortLcoreBinding portLcoreBinding[Mao_MAX_LCORE];




volatile bool MaoNeedExit;

//struct rte_lpm


static void
process_packet(struct rte_mbuf * pkt) {

}

static int
process_loop(void * notUse) {

    unsigned myLcoreId = rte_lcore_id();
    if (0 == portLcoreBinding[myLcoreId].count) {
        RTE_LOG(INFO, Mao, "nothing to do and released, lcore: %d, socketId: %d\n", rte_lcore_id(), rte_socket_id());
        return 0;

    } else {
        RTE_LOG(INFO, Mao, "hello, this is lcore: %d, socketId: %d, portCount: %d\n",
                rte_lcore_id(), rte_socket_id(), portLcoreBinding[myLcoreId].count);
    }

    struct rte_mbuf * rxBuffer[Mao_MAX_RX_BURST];

    unsigned portId;

    while (!MaoNeedExit) {


        for (unsigned i = 0; i < portLcoreBinding[myLcoreId].count; i++) {

            portId = portLcoreBinding[myLcoreId].ports[i];

            // Tx flush
            rte_eth_tx_buffer_flush(portId, 0, txBuffers[portId]);


            // Rx process
            unsigned rxCount = rte_eth_rx_burst(portId, 0, rxBuffer, Mao_MAX_RX_BURST);
            for (unsigned i = 0; i < rxCount; i++) {
                process_packet(rxBuffer[i]);
            }
        }
    }

    return -5;
}


static void
sys_signal_process(int signal) {

    switch (signal) {
        case SIGINT:
        case SIGTERM:
            MaoNeedExit = true;
            break;
        default:;
    }
}

static void
setup_ip_tunnel_table (void) {
//    rte_lpm_config

//    rte_lpm_create("ip-tunnel-mapping-table", rte_socket_id(),)

}


int
main(int argc, char ** argv) {


    int ret = 0;

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(ret, "Fail: RTE init\n");


    // register callback for system signal, e.g. Ctrl+C or else
    signal(SIGINT, sys_signal_process);
    signal(SIGTERM, sys_signal_process);


    rxMbufPool = rte_pktmbuf_pool_create(
            "mao-rx-buffer", Mao_NB_MBUF, Mao_MEMPOOL_CACHE_SIZE, 0, Mao_MBUF_BUF_SIZE, rte_socket_id());
    if (NULL == rxMbufPool)
        rte_exit(EXIT_FAILURE, "Fail: create Rx pktmbuf pool\n");





    unsigned portCount = rte_eth_dev_count();
    if (portCount > Mao_MAX_LCORE * Mao_MAX_PORT_PER_LCORE ||
        portCount > Mao_MAX_ETHPORTS)
        rte_exit(EXIT_FAILURE, "Fail: can not support so many ports with so few CPUs\n");


    // assign Eth dev to lcore
    for (unsigned portId = 0, lcoreId = 0; portId < portCount; portId++) {

        if (portLcoreBinding[lcoreId].count < Mao_MAX_PORT_PER_LCORE) {
            portLcoreBinding[lcoreId].ports[portLcoreBinding[lcoreId].count++] = portId;
        } else {
            if (++lcoreId < Mao_MAX_LCORE) {
                portId--; // for re-do this round of loop

            } else {
                // should exit after Get & Check portCount above
                rte_exit(EXIT_FAILURE, "Fail & Caution! - can not support so many ports with so few CPUs\n");
            }
        }
    }



    // init & start Eth dev
    struct rte_eth_dev_info devInfoTemp;
    for (unsigned portId = 0; portId < portCount; portId++) {


        ret = rte_eth_dev_configure(portId, 1, 1, &portConfigure);
        if (ret < 0)
            rte_exit(ret, "Fail: config eth dev, Port: %d\n", portId);



        rte_eth_dev_info_get(portId, &devInfoTemp);
        if (ret < 0)
            rte_exit(ret, "Fail: get eth dev info, Port: %d\n", portId);

        // rte_eth_dev_adjust_nb_rx_tx_desc(portId, )


        ret = rte_eth_rx_queue_setup(portId, 0, devInfoTemp.rx_desc_lim.nb_max, rte_socket_id(), NULL, rxMbufPool);
        if (ret < 0)
            rte_exit(ret, "Fail: setup Rx queue, Port: %d\n", portId);

        ret = rte_eth_tx_queue_setup(portId, 0, devInfoTemp.tx_desc_lim.nb_max, rte_socket_id(), NULL);
        if (ret < 0)
            rte_exit(ret, "Fail: setup Tx queue, Port: %d\n", portId);



        txBuffers[portId] = rte_zmalloc_socket(
                "mao-tx-buffer", RTE_ETH_TX_BUFFER_SIZE(Mao_TX_BUF_NB_MBUF), Mao_TX_BUF_ALIGN, rte_socket_id());
        if (NULL == txBuffers[portId])
            rte_exit(EXIT_FAILURE, "Fail: zmalloc Tx buffer, Port: %d\n", portId);


        ret = rte_eth_tx_buffer_init(txBuffers[portId], Mao_TX_BUF_NB_MBUF);
        if (ret < 0)
            rte_exit(ret, "Fail: init Tx buffer, Port: %d\n", portId);


        ret = rte_eth_dev_start(portId);
        if (ret < 0)
            rte_exit(ret, "Fail: start eth dev, Port: %d\n", portId);


        rte_eth_promiscuous_enable(portId);

        RTE_LOG(INFO, Mao, "Radar contact! Port: %d\n", portId);

    }


    // setup LPM
    setup_ip_tunnel_table();



    RTE_LOG(INFO, Mao, "Ready to do my work!\n");


    MaoNeedExit = false;

    rte_eal_mp_remote_launch(process_loop, NULL, CALL_MASTER);

    ret = 0;
    unsigned lcoreId;
    RTE_LCORE_FOREACH(lcoreId) {
        if (rte_eal_wait_lcore(lcoreId) < 0) {
            ret -= 1;
            // Mao: need 'break' here?
        }
    }





    for (unsigned portId = 0; portId < portCount; portId++) {
        RTE_LOG(INFO, Mao, "Closing port %d ...\n", portId);
        rte_eth_dev_stop(portId);
        rte_eth_dev_close(portId);
        RTE_LOG(INFO, Mao, "Closed port %d\n", portId);
    }


    // Mao Caution !!! Do we need to free them manually?
    for (unsigned portId = 0; portId < Mao_MAX_ETHPORTS; portId++) {
        if (txBuffers[portId] != NULL) {
            RTE_LOG(INFO, Mao, "free Tx buffer... , port: %d\n", portId);
            rte_free(txBuffers[portId]);
        }
    }

    RTE_LOG(INFO, Mao, "Good day! Return: %d\n", ret);

    return ret;
}
