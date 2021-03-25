#include <stdio.h>
#include <stdlib.h>
#include "../com.h"
#include "../littlefs/lfs.h"
#include "../flash.h"

int ls_fun(int argc, char *argv[])
{
    const char * path = NOW_PATH_BUF;
    int is_l = 0;
    static struct lfs_info info_v = {0};

    lfs_dir_t *dir = NULL;

    if(argc == 1)
    {
        if(strcmp("-l", argv[0]) == 0)
            is_l = 1;
        else
            path = argv[0];
    }
    else if(argc == 2)
    {
        if(strcmp("-l", argv[0]) == 0)
        {
            is_l = 1;
            path = argv[1];
        }
        else
        {
            if(strcmp("-l", argv[1]) == 0)
            {
                is_l = 1;
                path = argv[0];
            }
            else
            {
                printf("non-support parameter\n");
                return 0;
            }
        }
    }
    else if(argc > 2)
    {
        printf("Too many parameters\n");
        return 0;
    }

    if(path[0] != '/')
        sprintf(oper_path, "%s%s", NOW_PATH_BUF, path);
    else
        strcpy(oper_path, path);

    dir = dir_open(oper_path);
    if(dir == NULL)
    {
        printf("dir[%s] open failed\n", path);
        return 0;
    }

    while(1)
    {
        if(dir_read(dir, &info_v) != 0)
        {
            if(is_l == 0)
            {
                printf("%s\t", info_v.name);
            }
            else
            {
                if(info_v.type == LFS_TYPE_REG)
                    printf("-\t%d\t%s\n", info_v.size, info_v.name);
                else
                    printf("d\t1\t%s\n", info_v.name);
            }
        }
        else
        {
            break;
        }
    }
    if(is_l == 0)
    {
        printf("\n");
    }

    dir_close(dir);

    return 0;
}

cmd_t ls = {"ls", ls_fun, "ls\nls -l\n"};
