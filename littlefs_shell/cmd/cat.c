#include <stdio.h>
#include <stdlib.h>
#include "../com.h"
#include "../littlefs/lfs.h"
#include "../flash.h"

#define F_BOARD 0
#define F_PC    1

#define ONCE_RW_SIZE    4096

int cp_fun(int argc, char *argv[])
{
    int read_size = 0;
    int f_argv1 = 0;
    int f_argv2 = 0;

    unsigned char *file_buf = NULL;

    lfs_file_t * b_file1 = NULL;
    lfs_file_t * b_file2 = NULL;
    FILE * p_file1 = NULL;
    FILE * p_file2 = NULL;


    if(argc != 2)
    {
        printf("Too many parameters\n");
        return 0;
    }

    if(strncmp("PC:", argv[0], 3) == 0)
        f_argv1 = F_PC;
    else
        f_argv1 = F_BOARD;

    if(strncmp("PC:", argv[1], 3) == 0)
        f_argv2 = F_PC;
    else
        f_argv2 = F_BOARD;


    if(f_argv1 == F_BOARD)
    {
        if(argv[0][0] != '/')
            sprintf(oper_path, "%s%s", NOW_PATH_BUF, argv[0]);
        else
            strcpy(oper_path, argv[0]);

        b_file1 = file_open(oper_path, LFS_O_RDONLY);
        if(b_file1 == NULL)
        {
            printf("file1[%s] open failed\n", argv[0]);
            return 0;
        }
    }
    else
    {
        p_file1 = fopen(&(argv[0][3]), "rb");
        if(p_file1 == NULL)
        {
            printf("file1[%s] open failed\n", &(argv[0][3]));
            return 0;
        }
    }

    if(f_argv2 == F_BOARD)
    {
        if(argv[1][0] != '/')
            sprintf(oper_path, "%s%s", NOW_PATH_BUF, argv[1]);
        else
            strcpy(oper_path, argv[1]);

        b_file2 = file_open(oper_path, LFS_O_WRONLY | LFS_O_CREAT);
        if(b_file2 == NULL)
        {
            if(f_argv1 == F_BOARD)
            {
                file_close(b_file1);
            }
            else
            {
                fclose(p_file1);
            }
            printf("file2[%s] open failed\n", argv[1]);
            return 0;
        }
    }
    else
    {
        p_file2 = fopen(&(argv[1][3]), "wb");
        if(p_file2 == NULL)
        {
            if(f_argv1 == F_BOARD)
            {
                file_close(b_file1);
            }
            else
            {
                fclose(p_file1);
            }
            printf("file2[%s] open failed\n", &(argv[1][3]));
            return 0;
        }
    }

    file_buf = (unsigned char *)malloc(ONCE_RW_SIZE);
    if(file_buf == NULL)
    {
        if(f_argv1 == F_BOARD)
        {
            file_close(b_file1);
        }
        else
        {
            fclose(p_file1);
        }

        if(f_argv2 == F_BOARD)
        {
            file_close(b_file2);
        }
        else
        {
            fclose(p_file2);
        }

        printf("failed!\nsystem err!\n");
        return 0;
    }

    while(1)
    {
        if(f_argv1 == F_BOARD)
        {
            read_size = file_read(b_file1, file_buf, ONCE_RW_SIZE);
        }
        else
        {
            read_size = fread(file_buf, 1, ONCE_RW_SIZE, p_file1);
        }

        if(f_argv2 == F_BOARD)
        {
            file_write(b_file2, file_buf, read_size);
        }
        else
        {
            fwrite(file_buf, 1, read_size, p_file2);
        }

        if(read_size != ONCE_RW_SIZE)
            break;
    }

    if(f_argv1 == F_BOARD)
    {
        file_close(b_file1);
    }
    else
    {
        fclose(p_file1);
    }

    if(f_argv2 == F_BOARD)
    {
        file_close(b_file2);
    }
    else
    {
        fclose(p_file2);
    }

    free(file_buf);

    return 0;
}

cmd_t cp = {"cp", cp_fun, "cp PC:C:/user/a a\n"};
