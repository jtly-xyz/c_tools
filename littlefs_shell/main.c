#include <stdio.h>
#include <stdlib.h>
#include "com.h"
#include "littlefs/lfs.h"
#include "flash.h"

static char input_buf[1024] = "cp PC:E:\\temp\\QQ.png q2.png";

int main()
{
    char attr[10] = {0};
    int i = 0;

    if(init_lfs() != 0)
    {
        printf("mount failed\n");
        getch();
        return 0;
    }

    printf("mount succeed\n\n");

//    printf("LFS:%s# \n", NOW_PATH_BUF);
//    exec_cmd(input_buf);

    file_setattr("lfs_util.h", 0x00, "12-01", 5);

    if(file_getattr("lfs_util.h", 0x00, attr, 5) == 5)
    {
        printf("ATTR: %s\n", attr);
    }
    else
    {
        printf("get attr err\n");
    }

    while(1)
    {
        i = 0;
        printf("LFS:%s# ", NOW_PATH_BUF);
        while(1)
        {
            input_buf[i] = (char)getchar();
            if(input_buf[i] == '\n')
            {
                input_buf[i] = 0;
                break;
            }

            i++;
        }
        exec_cmd(input_buf);
    }

    return 0;
}
