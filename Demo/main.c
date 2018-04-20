
#include <rte_eal.h>
#include <stdbool.h>
#include <signal.h>
#include <rte_mbuf.h>
#include <rte_ethdev.h>
#include <rte_malloc.h>
#include "portConfigure.h"


#define Mao_TX_BUF_NB_MBUF 32 // number of MBUF in one Tx buffer
#define Mao_TX_BUF_ALIGN 64 // number of MBUF in one Tx buffer

#define Mao_NB_MBUF 8192 // number of MBUF
#define Mao_MEMPOOL_CACHE_SIZE RTE_MEMPOOL_CACHE_MAX_SIZE
#define Mao_MBUF_BUF_SIZE RTE_MBUF_DEFAULT_BUF_SIZE


struct rte_mempool * rxMbufPool;



struct rte_eth_dev_tx_buffer * txBuffers[RTE_MAX_ETHPORTS];



volatile bool MaoNeedExit;



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


int
main(int argc, char ** argv) {


    int ret = 0;

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(ret, "Mao - Fail: RTE init");



    signal(SIGINT, sys_signal_process);
    signal(SIGTERM, sys_signal_process);


    rxMbufPool = rte_pktmbuf_pool_create(
            "mao-rx-buffer", Mao_NB_MBUF, Mao_MEMPOOL_CACHE_SIZE, 0, Mao_MBUF_BUF_SIZE, rte_socket_id());
    if (NULL == rxMbufPool)
        rte_exit(EXIT_FAILURE, "Mao - Fail: create Rx pktmbuf pool");





    unsigned portCount = rte_eth_dev_count();

    struct rte_eth_dev_info devInfoTemp;
    for (unsigned portId = 0; portId < portCount; portId++) {


        ret = rte_eth_dev_configure(portId, 1, 1, &portConfigure);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: config eth dev, Port: %d", portId);



        rte_eth_dev_info_get(portId, &devInfoTemp);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: get eth dev info, Port: %d", portId);

        // rte_eth_dev_adjust_nb_rx_tx_desc(portId, )


        ret = rte_eth_rx_queue_setup(portId, 0, devInfoTemp.rx_desc_lim.nb_max, rte_socket_id(), NULL, rxMbufPool);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: setup Rx queue, Port: %d", portId);

        ret = rte_eth_tx_queue_setup(portId, 0, devInfoTemp.tx_desc_lim.nb_max, rte_socket_id(), NULL);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: setup Tx queue, Port: %d", portId);



        txBuffers[portId] = rte_zmalloc_socket(
                "mao-tx-buffer", RTE_ETH_TX_BUFFER_SIZE(Mao_TX_BUF_NB_MBUF), Mao_TX_BUF_ALIGN, rte_socket_id());
        if (NULL == txBuffers[portId])
            rte_exit(EXIT_FAILURE, "Mao - Fail: zmalloc Tx buffer, Port: %d", portId);


        ret = rte_eth_tx_buffer_init(txBuffers[portId], Mao_TX_BUF_NB_MBUF);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: init Tx buffer, Port: %d", portId);


        ret = rte_eth_dev_start(portId);
        if (ret < 0)
            rte_exit(ret, "Mao - Fail: start eth dev, Port: %d", portId);


        rte_eth_promiscuous_enable(portId);

        printf("Mao - Radar contact! Port: %d\n", portId);

    }

    printf("Mao - Ready to do my work!\n");


    MaoNeedExit = false;


    for (unsigned portId = 0; portId < portCount; portId++) {
        printf("Mao - Closing port %d ...\n", portId);
        rte_eth_dev_stop(portId);
        rte_eth_dev_close(portId);
        printf("Mao - Closed port %d\n", portId);
    }


    // Mao Caution !!! Do we need to free them manually?
    for (unsigned portId = 0; portId < RTE_MAX_ETHPORTS; portId++) {
        if (txBuffers[portId] != NULL) {
            printf("Mao - free Tx buffer... , port: %d\n", portId);
            rte_free(txBuffers[portId]);
        }
    }

    printf("Mao - Good day! Return: %d\n", ret);

    return ret;
}
