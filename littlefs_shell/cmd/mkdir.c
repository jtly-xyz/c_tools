#include "../com.h"
#include "../littlefs/lfs.h"
#include "../flash.h"

int mkdir_fun(int argc, char *argv[])
{
    if(argc != 1)
    {
        printf("Too many parameters\n");
        return 0;
    }

    if(argv[0][0] != '/')
        sprintf(oper_path, "%s%s", NOW_PATH_BUF, argv[0]);
    else
        strcpy(oper_path, argv[0]);

    if(dir_mkdir(oper_path) != LFS_ERR_OK)
    {
        printf("failed!\n");
    }

    return 0;
}

cmd_t mkdir = {"mkdir", mkdir_fun, "ls\nls -l\n"};
