#include <stdio.h>
#include <string.h>
#include <io.h>


unsigned char f1_buf[1*1024*1024] = {0};
unsigned char f2_buf[1*1024*1024] = {0};

char  file1_p[1024];
char  file2_p[1024];

int main(int argc, char ** argv)
{
    FILE *f_file1 = NULL;
    FILE *f_file2 = NULL;

    long long file1_size = 0;
    long long file2_size = 0;
    long long valid_size = 0;

    int r_size = 0;

    int i = 0;

    printf("�������ļ��Աȹ��� v0.1 author:swc \r\n");
    printf("�������ļ�1·��\r\n");
    scanf("%s", file1_p);
    printf("�������ļ�2·��\r\n");
    scanf("%s", file2_p);

    f_file1 = fopen(file1_p, "rb");
    if(f_file1 == NULL)
    {
        printf("�ļ�1�����ڣ�\r\n");
        printf("��������˳�\r\n");
        getch();
        return 0;
    }
    f_file2 = fopen(file2_p, "rb");
    if(f_file2 == NULL)
    {
        fclose(f_file1);
        printf("�ļ�2�����ڣ�\r\n");
        printf("��������˳�\r\n");
        getch();
        return 0;
    }

    printf("��ʼ�Ա� \r\n");

	fseek(f_file1, 0, SEEK_END);
	file1_size = ftell(f_file1);

    fseek(f_file2, 0, SEEK_END);
	file2_size = ftell(f_file2);

	if(file1_size != file2_size)
    {
        fclose(f_file1);
        fclose(f_file2);
        printf("\r\n**** �ļ�1���ļ�2����ͬ��****\r\n");
        printf("��������˳�\r\n");
        getch();
        return 0;
    }

    fseek(f_file1, 0, SEEK_SET);
    fseek(f_file2, 0, SEEK_SET);

    fread(f1_buf, 1, 1*1024*1024, f_file1);
    r_size = fread(f2_buf, 1, 1*1024*1024, f_file2);

    while(r_size)
    {
        for(i=0; i<r_size; i++)
        {
            if(f1_buf[i] != f2_buf[i])
            {
                fclose(f_file1);
                fclose(f_file2);
                printf("\r\n\r\n**** �ļ�1���ļ�2����ͬ��****\r\n");
                printf("\r\n��������˳�\r\n");
                getch();
                return 0;
            }
        }

        valid_size += r_size;
        if(valid_size <= 1*1024*1024)
            printf("�����%02d%%", valid_size * 100 / file1_size);
        else
            printf("\b\b\b%02d%%", valid_size * 100 / file1_size);

        fread(f1_buf, 1, 1*1024*1024, f_file1);
        r_size = fread(f2_buf, 1, 1*1024*1024, f_file2);
    }

    fclose(f_file1);
    fclose(f_file2);
    printf("\r\n\r\n**** �ļ�1���ļ�2��ȫ��ͬ��****\r\n");
    printf("\r\n��������˳�\r\n");
    getch();

    return 0;
}



