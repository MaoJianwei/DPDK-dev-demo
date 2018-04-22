//
// Created by mao on 18-4-22.
//

#include <stdint.h>
#include <rte_malloc.h>

#include "MaoIpRouteTableLPM.h"
#include "MaoConstant.h"

struct MaoRouteNode *
ip_tunnel_mapping_create() {

    struct MaoRouteNode * ipTunnulTable =
            (MaoRouteNode *)rte_zmalloc("mao-ip-tunnel-table", sizeof(MaoRouteNode), Mao_MALLOC_ALIGN);
    return ipTunnulTable;
}

void
ip_tunnel_mapping_add(struct MaoRouteNode * maoTunnelTable,
                      uint32_t ip, uint32_t prefixLen, struct MaoMpls * MaoMplsTunnel) {
    
}

struct MaoMpls *
ip_tunnel_mapping_delete(struct MaoRouteNode * maoTunnelTable, uint32_t ip, uint32_t prefixLen) {

}

struct MaoMpls *
ip_tunnel_mapping_lookup(struct MaoRouteNode * maoTunnelTable, uint32_t ip) {

}

void
ip_tunnel_mapping_free(struct MaoRouteNode * maoTunnelTable) {

}