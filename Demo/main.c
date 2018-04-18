
#include <rte_eal.h>

int
main(int argc, char ** argv) {


    int ret = 0;

    ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Mao - RTE init fail, return: %d", ret);
    }

}
