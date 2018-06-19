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


// TODO: all below


struct MaoRouteNode *
ip_tunnel_mapping_create(void);

void
ip_tunnel_mapping_add(struct MaoRouteNode * maoTunnelTable,
                      uint32_t ip, uint32_t prefixLen, struct MaoMplsTunnel * MaoMplsTunnel);

struct MaoMpls *
ip_tunnel_mapping_delete(struct MaoRouteNode * maoTunnelTable, uint32_t ip, uint32_t prefixLen);

struct MaoMpls *
ip_tunnel_mapping_lookup(struct MaoRouteNode * maoTunnelTable, uint32_t ip);

void
ip_tunnel_mapping_free(struct MaoRouteNode * maoTunnelTable);


#endif //DPDK_STABLE_17_11_1_IPROUTETABLELPM_H
