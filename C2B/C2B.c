#include <stdio.h>
#include <string.h>
#include <io.h>

unsigned char *f_buf;
unsigned char _f_buf[10*1024+20] = {0};
unsigned char t_buf[10*1024] = {0};

char    to_p_buf[4096] = "default.bin";
const char * from_p = "default.c";
char * to_p = to_p_buf;

int main(int argc, char ** argv)
{
    FILE *f_from = NULL;
    FILE *f_to = NULL;

    int p_len_temp1 = 0;
    int p_len_temp2 = 0;

    int _r_size = 0;
    int r_size = 0;
    int valid_size = 0;
    int i = 0;

    int residue = 0;

    f_buf = _f_buf;

    unsigned char temp[3] = {0};


    printf("C��������ת�������ļ� C2B_v0.1 author:swc \r\n");

    if(argc == 2)
    {
        from_p = argv[1];
        strcpy(to_p_buf, argv[1]);

        p_len_temp1 = strlen(to_p_buf);
        p_len_temp2 = p_len_temp1;

        while((to_p_buf[p_len_temp1] != '/') && (to_p_buf[p_len_temp1] != '\\') && (to_p_buf[p_len_temp1] != '.') && p_len_temp1)
            p_len_temp1--;

        if(to_p_buf[p_len_temp1] == '.')
        {
            to_p_buf[p_len_temp1 + 1] = 'b';
            to_p_buf[p_len_temp1 + 2] = 'i';
            to_p_buf[p_len_temp1 + 3] = 'n';
            to_p_buf[p_len_temp1 + 4] = 0;
        }
        else
        {
            to_p_buf[p_len_temp2] = '.';
            to_p_buf[p_len_temp2 + 1] = 'b';
            to_p_buf[p_len_temp2 + 2] = 'i';
            to_p_buf[p_len_temp2 + 3] = 'n';
            to_p_buf[p_len_temp2 + 4] = 0;
        }

        to_p = to_p_buf;
    }
    else if(argc == 3)
    {
        from_p = argv[1];
        to_p = argv[2];
    }
    else if(argc > 3)
    {
        printf("���󣬲�������\r\n");
        printf("��������˳�\r\n");
        getch();
        return 0;
    }




    f_from = fopen(from_p, "r");
    if(f_from == NULL)
    {
        printf("�����Ҳ����ļ���\r\n");
        printf("ʹ�÷���1:ֱ��˫�����У�ת��Ĭ���ļ�default.c\r\n");
        printf("ʹ�÷���2:��Ҫת�����ļ����뵽���ļ�\r\n");
        printf("ʹ�÷���3:��cmd��ִ�и��ļ�������Ҫת�����ļ�����ת������ļ�������ѡ���Բ�������ʽ����\r\n");
        printf("��������˳�\r\n");
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
                to_p[p_len_temp1 - 4] = '(';
                to_p[p_len_temp1 - 3] = '0' + num;
                to_p[p_len_temp1 - 2] = ')';
                to_p[p_len_temp1 - 1] = '.';
                to_p[p_len_temp1 + 0] = 'b';
                to_p[p_len_temp1 + 1] = 'i';
                to_p[p_len_temp1 + 2] = 'n';
                to_p[p_len_temp1 + 3] = 0;
                first = 1;
            }
            else
            {
                if(num < 10)
                {
                    to_p[p_len_temp1 - 3] = '0' + num;
                    to_p[p_len_temp1 - 2] = ')';
                    to_p[p_len_temp1 - 1] = '.';
                    to_p[p_len_temp1 + 0] = 'b';
                    to_p[p_len_temp1 + 1] = 'i';
                    to_p[p_len_temp1 + 2] = 'n';
                    to_p[p_len_temp1 + 3] = 0;
                }
                else if(num < 100)
                {
                    to_p[p_len_temp1 - 3] = '0' + num / 10;
                    to_p[p_len_temp1 - 2] = '0' + num % 10;
                    to_p[p_len_temp1 - 1] = ')';
                    to_p[p_len_temp1 + 0] = '.';
                    to_p[p_len_temp1 + 1] = 'b';
                    to_p[p_len_temp1 + 2] = 'i';
                    to_p[p_len_temp1 + 3] = 'n';
                    to_p[p_len_temp1 + 4] = 0;
                }
                else if(num < 1000)
                {
                    to_p[p_len_temp1 - 3] = '0' + num / 100;
                    to_p[p_len_temp1 - 2] = '0' + num % 100 / 10;
                    to_p[p_len_temp1 - 1] = '0' + num % 10;
                    to_p[p_len_temp1 + 0] = ')';
                    to_p[p_len_temp1 + 1] = '.';
                    to_p[p_len_temp1 + 2] = 'b';
                    to_p[p_len_temp1 + 3] = 'i';
                    to_p[p_len_temp1 + 4] = 'n';
                    to_p[p_len_temp1 + 5] = 0;
                }
            }
        }
    }


    f_to = fopen(to_p, "wb");
    if(f_to == NULL)
    {
        printf("����дʧ�ܣ�\r\n");
        printf("��������˳�\r\n");
        getch();
        return 0;
    }

    printf("ת���С���\r\n");
    do
    {
        valid_size = 0;

        _r_size = fread(f_buf+residue, 1, 10*1024, f_from);
        r_size = _r_size + residue;
        for(i=0; i<(r_size - 3); i++)
        {
            if(f_buf[i] == '0' && ((f_buf[i+1] == 'x') || (f_buf[i+1] == 'X')))
            {
                temp[0] = f_buf[i+2];
                temp[1] = f_buf[i+3];

                t_buf[valid_size] = ((temp[0] & 0x40) ? (temp[0] & 0x20 ? temp[0] - 'a': temp[0] - 'A') + 10 : (temp[0] - '0')) << 4;
                t_buf[valid_size] |= (temp[1] & 0x40) ? (temp[1] & 0x20 ? temp[1] - 'a': temp[1] - 'A') + 10 : (temp[1] - '0');
                valid_size++;
            }
        }

        f_buf[0] = f_buf[r_size - 3];
        f_buf[1] = f_buf[r_size - 2];
        f_buf[2] = f_buf[r_size - 1];
        residue = 3;

        fwrite(t_buf, 1, valid_size, f_to);

    }while(_r_size);


    fclose(f_from);
    fclose(f_to);

    return 0;
}


