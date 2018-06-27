//
// Created by mao on 18-4-22.
//

#include <stdint.h>
#include <rte_malloc.h>
#include <RouterParam/TableConfigure.h>
#include <rte_lpm.h>
#include <stdbool.h>

#include "MaoIpRouteTableLPM.h"
#include "RouterParam/MaoConstant.h"


// not Thread-Security

volatile bool tableAvailable = false;
struct rte_lpm * ipTunnelTable = NULL;

int tunnelValidCount = 0;
int tunnelWritePointer = 0;
struct MaoMplsLabelNode ** tunnelStore = NULL;





struct MaoRouteNode *
ip_tunnel_table_create(void) {

//    struct MaoRouteNode * ipTunnulTable =
//            (struct MaoRouteNode *)rte_zmalloc("mao-ip-tunnel-table", sizeof(struct MaoRouteNode), Mao_MALLOC_ALIGN);
//    return ipTunnulTable;

    if (NULL != ipTunnelTable || NULL != tunnelStore)
        rte_exit(EXIT_FAILURE, "Fail: mao-ip-to-tunnel OR mao-mpls-tunnel-store existed.\n");


    tableAvailable = false;
    //TODO - fit multiple socketId
    ipTunnelTable = rte_lpm_create("mao-ip-to-tunnel", 0, &ipTunnelTableConfig);


    struct MaoMplsLabelNode ** tunnelStore = rte_zmalloc("mao-mpls-tunnel-store", Mao_IPTABLE_MAX_RULES* sizeof(void *), Mao_IPTABLE_ALIGN);
    if (NULL == tunnelStore)
        rte_exit(EXIT_FAILURE, "Fail: create mao-mpls-tunnel-store table\n");
    tunnelValidCount = 0;
    tunnelWritePointer = 0;
    tableAvailable = true;
}

void
ip_tunnel_table_add(struct MaoRouteNode *maoTunnelTable,
                    uint32_t ip, uint32_t prefixLen, struct MaoMplsTunnel *MaoMplsTunnel) {

}

struct MaoMpls *
ip_tunnel_table_delete(struct MaoRouteNode *maoTunnelTable, uint32_t ip, uint32_t prefixLen) {

}

struct MaoMpls *
ip_tunnel_table_lookup(struct MaoRouteNode *maoTunnelTable, uint32_t ip) {

}

static void
freeOneMplsTunnel(unsigned pos) {
    struct MaoMplsLabelNode * cur = tunnelStore[pos];
    struct MaoMplsLabelNode * next;
    while(cur != NULL) {
        next = cur->nextLabel;
        free(cur);
        cur = next;
    }
}

void
ip_tunnel_table_free(struct MaoRouteNode *maoTunnelTable) {

    tableAvailable = false;

    unsigned cleanPointer = 0;
    while (tunnelValidCount > 0) {
        while (tunnelStore[cleanPointer] == NULL) {
            cleanPointer = (cleanPointer + 1) % Mao_IPTABLE_MAX_RULES;
        }
        freeOneMplsTunnel(cleanPointer);
        tunnelValidCount--;
    }


    rte_lpm_delete_all(ipTunnelTable);
    rte_lpm_free(ipTunnelTable);
}
