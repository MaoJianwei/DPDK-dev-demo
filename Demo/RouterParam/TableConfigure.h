//
// Created by mao on 18-6-23.
//

#ifndef DPDK_STABLE_17_11_1_TABLECONFIGURE_H
#define DPDK_STABLE_17_11_1_TABLECONFIGURE_H

#include "MaoConstant.h"

struct rte_lpm_config ipTunnelTableConfig = {
        .max_rules = Mao_IPTABLE_MAX_RULES,
        .number_tbl8s = Mao_IPTABLE_NB_TBL8,
        .flags = 0;
};

#endif //DPDK_STABLE_17_11_1_TABLECONFIGURE_H
