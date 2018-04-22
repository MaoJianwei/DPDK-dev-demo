//
// Created by mao on 18-4-22.
//

#ifndef DPDK_STABLE_17_11_1_MAOCONSTANT_H
#define DPDK_STABLE_17_11_1_MAOCONSTANT_H




#define RTE_LOGTYPE_Mao RTE_LOGTYPE_USER1


#define Mao_NB_MBUF 8192 // number of MBUF
#define Mao_MEMPOOL_CACHE_SIZE RTE_MEMPOOL_CACHE_MAX_SIZE
#define Mao_MBUF_BUF_SIZE RTE_MBUF_DEFAULT_BUF_SIZE

#define Mao_TX_BUF_NB_MBUF 32 // number of MBUF in one Tx buffer
#define Mao_TX_BUF_ALIGN 64 // packet alignment for Tx buffer
#define Mao_MALLOC_ALIGN Mao_TX_BUF_ALIGN

#define Mao_MAX_RX_BURST Mao_TX_BUF_NB_MBUF

#define Mao_MAX_LCORE 16 // RTE_MAX_LCORE 128
#define Mao_MAX_ETHPORTS 16 // RTE_MAX_ETHPORTS 32






#endif //DPDK_STABLE_17_11_1_MAOCONSTANT_H
