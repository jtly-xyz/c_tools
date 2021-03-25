#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include "tchar.h"

const unsigned char ack_s[] = {0x04, 0xD0, 0x00, 0x00, 0xFF, 0x2C};
const unsigned char ack_m[] = {0x04, 0xD0, 0x04, 0x00, 0xFF, 0x28};
const unsigned char imager_cmd[] = {0x05, 0xf7, 0x04, 0x00, 0x01, 0xfe, 0xff};
const unsigned char tig_cmd[] = {0x04, 0xE4, 0x04, 0x00, 0xFF, 0x14};

int main()
{
    HANDLE hCom = NULL;
    hCom = CreateFile(_T("COM9"),
            GENERIC_READ | GENERIC_WRITE,
            0,//独占方式
            NULL,
            OPEN_EXISTING,//打开而不是创建
            0,//同步方式
            NULL);
    if(hCom == (HANDLE)-1)
    {
        printf("打开COM失败!\n");
        getchar();
        return FALSE;
    }
    else
    {
        printf("COM打开成功！\n");
    }
    SetupComm(hCom, 20480, 20480);//输入缓冲区和输出缓冲区的大小都是1024

    COMMTIMEOUTS TimeOuts;//设定读超时
    TimeOuts.ReadIntervalTimeout = 10;
    TimeOuts.ReadTotalTimeoutMultiplier = 10;
    TimeOuts.ReadTotalTimeoutConstant = 2000;
    TimeOuts.WriteTotalTimeoutMultiplier = 50;//设定写超时
    TimeOuts.WriteTotalTimeoutConstant = 500;
    SetCommTimeouts(hCom, &TimeOuts);//设置超时

    DCB dcb1;
    GetCommState(hCom, &dcb1);
    dcb1.BaudRate = 115200;//波特率为9600
    dcb1.ByteSize = 8;//每个字节有8位
    dcb1.Parity = NOPARITY;//无奇偶校验位
    dcb1.StopBits = ONESTOPBIT;//两个停止位
    dcb1.fParity = FALSE;
    dcb1.fNull = FALSE;
    SetCommState(hCom, &dcb1);

    DWORD wCount = 0;
    DWORD rCount = 0;

    PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);//清空缓冲区

    int image_len = 0;
    unsigned char *image_buf = NULL;
    image_buf = (unsigned char *)malloc(40*1024);
    if(image_buf == NULL)
    {
        printf("内存申请失败\n");
        getchar();
        return FALSE;
    }
    memset(image_buf, 0, 40*1024);

    unsigned char str[300] = {0};

    WriteFile(hCom, imager_cmd, sizeof(imager_cmd), &wCount, NULL);
    if(!ReadFile(hCom, str, 6, &rCount, NULL))
    {
        printf("读串口失败1!");
        getchar();
        return FALSE;
    }
    if(memcmp(str, ack_s, 6))
    {
        printf("失败2\n");
        getchar();
        return FALSE;
    }

   Sleep(20);

    WriteFile(hCom, tig_cmd, sizeof(tig_cmd), &wCount, NULL);
    if(!ReadFile(hCom, str, 6, &rCount, NULL))
    {
        printf("读串口失败3!");
        getchar();
        return FALSE;
    }
    if(memcmp(str, ack_s, 6))
    {
        printf("失败4\n");
        getchar();
        return FALSE;
    }


    if(!ReadFile(hCom, str, 256, &rCount, NULL))
    {
        printf("读串口失败5!");
        getchar();
        return FALSE;
    }
    if(rCount != 256)
    {
        printf("失败6\n");
        getchar();
        return FALSE;
    }
    memcpy(image_buf, &(str[14]), 240);
    image_len = 240;
    WriteFile(hCom, ack_m, sizeof(ack_m), &wCount, NULL);


    while(1)
    {
        if(!ReadFile(hCom, str, 256, &rCount, NULL))
        {
            printf("读串口失败7!");
            getchar();
            return FALSE;
        }
        if(rCount == 256)
        {
            memcpy(image_buf+image_len, &(str[4]), 250);
            image_len += 250;
            WriteFile(hCom, ack_m, sizeof(ack_m), &wCount, NULL);
            continue;
        }
        memcpy(image_buf+image_len, &(str[4]), rCount - 6);
        image_len += rCount - 6;
        WriteFile(hCom, ack_m, sizeof(ack_m), &wCount, NULL);
        break;
    }


    FILE *fp1;


    fp1 = fopen("tupian.jpg", "wb");
    fwrite(image_buf, 1, image_len, fp1);
    fclose(fp1);

    free(image_buf);

    CloseHandle(hCom);

    system("start tupian.jpg");

    return 0;
}
