//
// Created by mao on 18-4-22.
//

#ifndef DPDK_STABLE_17_11_1_IPROUTETABLELPM_H
#define DPDK_STABLE_17_11_1_IPROUTETABLELPM_H

#include "MaoMplsTunnel.h"

struct MaoRouteNode {

    struct MaoMplsTunnel * prefixBitLen[5]; // [0] is useless. surplus prefix length 1~4.

    struct MaoRouteNode * nextPrefix[16]; // 0000~1111

};




#endif //DPDK_STABLE_17_11_1_IPROUTETABLELPM_H
