#include "cmd.h"
#include "log.h"

int main(void) {

    log_init();

    while (1) {
        cmd_update();
    }

    return 0;
}
