#include "mobile.h"

int main(int argc, const char *argv[])
{
    mobile_t *mobile = malloc(sizeof(mobile_t));
    uv_loop_t *loop = uv_default_loop();

    mobile_init(mobile, loop, NULL);

    mobile->start(mobile);

    return uv_run(loop, UV_RUN_DEFAULT);
}
