cmake_minimum_required(VERSION 3.10)
project(DPDK_dev_demo)

set(CMAKE_CXX_STANDARD 11)

include_directories(Demo)

add_executable(DPDK_dev_demo
        Demo/main.c
        Demo/IpSrTunnel/MaoIpRouteTableLPM.c
        Demo/IpSrTunnel/MaoIpRouteTableLPM.h
        Demo/IpSrTunnel/MaoMplsTunnel.h
        Demo/SegmentRouting/mplsForwardTable.c
        Demo/SegmentRouting/mplsForwardTable.h
        Demo/RouterParam/PortConfigure.h
        Demo/RouterParam/PortLcoreBinding.h
        dpdk-example/main.c)
