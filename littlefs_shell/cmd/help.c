#include "../com.h"

int help_fun(int argc, char *argv[])
{
    if(argc > 0)
    {
        printf("Too many parameters\n");
        return 0;
    }


    return 0;
}

cmd_t help = {"help", help_fun, "ls\nls -l\n"};
