#include "cmd.h"
#include "log.h"

int main(void) {

    log_init();

    //cmd_prompt();   // ✅ print first prompt

    while (1) {
        cmd_update();
    }
}
