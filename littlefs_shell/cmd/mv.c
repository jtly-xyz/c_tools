#include "../com.h"

int mv_fun(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Too many parameters\n");
        return 0;
    }

    return 0;
}

cmd_t mv = {"mv", mv_fun, "ls\nls -l\n"};
