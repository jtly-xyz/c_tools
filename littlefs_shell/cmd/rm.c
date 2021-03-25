#include <stdio.h>
#include <stdlib.h>
#include "../com.h"
#include "../littlefs/lfs.h"
#include "../flash.h"



int rm_fun(int argc, char *argv[])
{
    int i = 0;

    if(argc == 0)
    {
        printf("Too many parameters\n");
        return 0;
    }

    for(i=0; i < argc; i++)
    {
        if(argv[i][0] != '/')
        {
            sprintf(oper_path, "%s%s", NOW_PATH_BUF, argv[i]);
        }
        else
        {
            strcpy(oper_path, argv[i]);
        }

        if(file_remove(oper_path) != LFS_ERR_OK)
        {
            printf("[%s]rm failed\n", argv[i]);
        }
    }

    return 0;
}

cmd_t rm = {"rm", rm_fun, "ls\nls -l\n"};
