#include <stdio.h>
#include <stdlib.h>
#include "../com.h"
#include "../littlefs/lfs.h"
#include "../flash.h"

int cd_fun(int argc, char *argv[])
{
    int i = 0;
    lfs_dir_t *dir = NULL;
    static struct lfs_info info_v = {0};
    char * tmp_to = NULL;
    char * tmp_from = NULL;


    if(argc > 1)
    {
        printf("Too many parameters\n");
        return 0;
    }

    if((strcmp(argv[0], "..") == 0) || (strcmp(argv[0], "../") == 0))
    {
        for(i=0; i<63; i++)
        {
            if(NOW_PATH_ARRY[i] == NULL)
                break;
        }
        if(i == 1)
        {
            return 0;
        }
        else
        {
            free(NOW_PATH_ARRY[i - 1]);
            NOW_PATH_ARRY[i - 1] = NULL;
        }

        tmp_to = NOW_PATH_BUF;
        for(i=0; i<64; i++)
        {
            if(NOW_PATH_ARRY[i] != NULL)
            {
                tmp_from = NOW_PATH_ARRY[i];
                while(*(tmp_to++) = *(tmp_from++));
                tmp_to--;
                if(i != 0)
                    *(tmp_to++) = '/';
            }
            else
            {
                *(tmp_to++) = 0;
                dir_close(dir);
                return 0;
            }
        }
    }

    dir = dir_open(NOW_PATH_BUF);
    if(dir == NULL)
    {
        printf("system err\n");
        return 0;
    }

    while(1)
    {
        if(dir_read(dir, &info_v) != 0)
        {
            if(info_v.type == LFS_TYPE_DIR)
            {
                if(strcmp(argv[0], info_v.name) == 0)
                {
                    for(i=0; i<63; i++)
                    {
                        if(NOW_PATH_ARRY[i] == NULL)
                            break;
                    }
                    if(i == 63)
                    {
                        printf("PATH too long\n");
                        return 0;
                    }
                    NOW_PATH_ARRY[i] = (char *)malloc(strlen(info_v.name) + 1);

                    strcpy(NOW_PATH_ARRY[i], info_v.name);

                    tmp_to = NOW_PATH_BUF;
                    for(i=0; i<64; i++)
                    {
                        if(NOW_PATH_ARRY[i] != NULL)
                        {
                            tmp_from = NOW_PATH_ARRY[i];
                            while(*(tmp_to++) = *(tmp_from++));
                            tmp_to--;
                            if(i != 0)
                                *(tmp_to++) = '/';
                        }
                        else
                        {
                            *(tmp_to++) = 0;
                            dir_close(dir);
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            printf("NOT found dir[%s]\n", argv[0]);
        }
    }

    dir_close(dir);



    return 0;
}

cmd_t cd = {"cd", cd_fun, "ls\nls -l\n"};
