//
// Created by mao on 18-4-20.
//

#ifndef DPDK_STABLE_17_11_1_PORTLCOREBINDING_H
#define DPDK_STABLE_17_11_1_PORTLCOREBINDING_H


#include <rte_memory.h>



#define Mao_MAX_PORT_PER_LCORE 1 // number of Port to each lcore


struct PortLcoreBinding {

    unsigned count;
    unsigned ports[Mao_MAX_PORT_PER_LCORE];

} __rte_cache_aligned;

#endif //DPDK_STABLE_17_11_1_PORTLCOREBINDING_H
