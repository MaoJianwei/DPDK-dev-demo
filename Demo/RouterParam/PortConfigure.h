
#ifndef DPDK_STABLE_17_11_1_PORTCONFIGURE_H
#define DPDK_STABLE_17_11_1_PORTCONFIGURE_H

#include <rte_ethdev.h>


struct rte_eth_conf portConfigure = {
        .rxmode = {
                .offloads = DEV_RX_OFFLOAD_CHECKSUM // DEV_RX_OFFLOAD_CRC_STRIP | DEV_RX_OFFLOAD_CHECKSUM
        },
        .txmode = {
                .mq_mode = ETH_MQ_TX_NONE,
                .offloads = DEV_TX_OFFLOAD_IPV4_CKSUM | DEV_TX_OFFLOAD_TCP_CKSUM | DEV_TX_OFFLOAD_UDP_CKSUM,
        }
};

#endif //DPDK_STABLE_17_11_1_PORTCONFIGURE_H
