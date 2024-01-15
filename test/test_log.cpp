#include "log.h"


int main() {
    LOGI("Info log test.");
    LOGI("Info log test %d.", 2);

    LOGE("Error log test.");
    LOGE("Error log test %d.", 2);
    return 0;
}

