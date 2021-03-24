#include <stdio.h>
#include <string.h>
#include <io.h>

unsigned char f_buf[16] = {0};
unsigned char t_buf[160] = {0};

char    to_buf[4096] = "default.c";
const char * from_p = "default.bin";
char * to_p = to_buf;



int main(int argc, char ** argv)
{
    FILE *f_from = NULL;
    FILE *f_to = NULL;

    int p_len_temp1 = 0;
    int p_len_temp2 = 0;

    int _r_size = 0;

    int i = 0, res = 0, w_size = 0;

    printf("任意文件转C语言数组 B2C_v0.1 author:swc \r\n");

    if(argc == 2)
    {
        from_p = argv[1];
        strcpy(to_buf, argv[1]);

        p_len_temp1 = strlen(to_buf);
        p_len_temp2 = p_len_temp1;

        while((to_buf[p_len_temp1] != '/') && (to_buf[p_len_temp1] != '\\') && (to_buf[p_len_temp1] != '.') && p_len_temp1)
            p_len_temp1--;

        if(to_buf[p_len_temp1] == '.')
        {
            to_buf[p_len_temp1 + 1] = 'c';
            to_buf[p_len_temp1 + 2] = 0;
        }
        //else if(to_buf[p_len_temp1] == '\\')
        else
        {
            to_buf[p_len_temp2] = '.';
            to_buf[p_len_temp2 + 1] = 'c';
            to_buf[p_len_temp2 + 2] = 0;
        }

        to_p = to_buf;
    }
    else if(argc == 3)
    {
        from_p = argv[1];
        to_p = argv[2];
    }
    else if(argc > 3)
    {
        printf("错误，参数过多\r\n");
        printf("按任意键退出\r\n");
        getch();
        return 0;
    }





    f_from = fopen(from_p, "rb");
    if(f_from == NULL)
    {
        printf("错误，找不到文件！\r\n");
        printf("使用方法1:直接双击运行，转换默认文件default.bin\r\n");
        printf("使用方法2:将要转换的文件拖入到该文件\r\n");
        printf("使用方法3:在cmd中执行该文件，并将要转换的文件名和转换后的文件名（可选）以参数的形式传入\r\n");
        printf("按任意键退出\r\n");
        getch();
        return 0;
    }

    int  first = 0;
    int  num = 0;

    if(argc < 3)
    {
        while(access(to_p, 0) == 0)
        {
            num++;
            if(first == 0)
            {
                p_len_temp1 = strlen(to_p);
                to_p[p_len_temp1 - 2] = '(';
                to_p[p_len_temp1 - 1] = '0' + num;
                to_p[p_len_temp1] = ')';
                to_p[p_len_temp1 + 1] = '.';
                to_p[p_len_temp1 + 2] = 'c';
                to_p[p_len_temp1 + 3] = 0;
                first = 1;
            }
            else
            {
                if(num < 10)
                {
                    to_p[p_len_temp1 - 1] = '0' + num;
                    to_p[p_len_temp1] = ')';
                    to_p[p_len_temp1 + 1] = '.';
                    to_p[p_len_temp1 + 2] = 'c';
                    to_p[p_len_temp1 + 3] = 0;
                }
                else if(num < 100)
                {
                    to_p[p_len_temp1 - 1] = '0' + num / 10;
                    to_p[p_len_temp1] = '0' + num % 10;
                    to_p[p_len_temp1 + 1] = ')';
                    to_p[p_len_temp1 + 2] = '.';
                    to_p[p_len_temp1 + 3] = 'c';
                    to_p[p_len_temp1 + 4] = 0;
                }
                else if(num < 1000)
                {
                    to_p[p_len_temp1 - 1] = '0' + num / 100;
                    to_p[p_len_temp1] = '0' + num % 100 / 10;
                    to_p[p_len_temp1 + 1] = '0' + num % 10;
                    to_p[p_len_temp1 + 2] = ')';
                    to_p[p_len_temp1 + 3] = '.';
                    to_p[p_len_temp1 + 4] = 'c';
                    to_p[p_len_temp1 + 5] = 0;
                }
            }
        }
    }

    f_to = fopen(to_p, "w");
    if(f_to == NULL)
    {
        printf("错误，写失败！\r\n");
        printf("按任意键退出\r\n");
        getch();
        return 0;
    }

    printf("转换中……\r\n");

    fwrite("const unsigned char Array[] = {\n", 1, strlen("const unsigned char Array[] = {\n"), f_to);
    do
    {
        _r_size = fread(f_buf, 1, 16, f_from);

        res = 0;
        w_size = 0;
        for(i = 0; i<_r_size; i++)
        {
            res = sprintf(&(t_buf[w_size]), "0x%02x, ", f_buf[i]);
            w_size += res;
        }
        t_buf[w_size] = '\n';

        fwrite(t_buf, 1, w_size+1, f_to);

    }while(_r_size);


    fwrite("};\n", 1, strlen("};\n"), f_to);

    fclose(f_from);
    fclose(f_to);

    return 0;
}




