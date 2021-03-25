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
            0,//��ռ��ʽ
            NULL,
            OPEN_EXISTING,//�򿪶����Ǵ���
            0,//ͬ����ʽ
            NULL);
    if(hCom == (HANDLE)-1)
    {
        printf("��COMʧ��!\n");
        getchar();
        return FALSE;
    }
    else
    {
        printf("COM�򿪳ɹ���\n");
    }
    SetupComm(hCom, 20480, 20480);//���뻺����������������Ĵ�С����1024

    COMMTIMEOUTS TimeOuts;//�趨����ʱ
    TimeOuts.ReadIntervalTimeout = 10;
    TimeOuts.ReadTotalTimeoutMultiplier = 10;
    TimeOuts.ReadTotalTimeoutConstant = 2000;
    TimeOuts.WriteTotalTimeoutMultiplier = 50;//�趨д��ʱ
    TimeOuts.WriteTotalTimeoutConstant = 500;
    SetCommTimeouts(hCom, &TimeOuts);//���ó�ʱ

    DCB dcb1;
    GetCommState(hCom, &dcb1);
    dcb1.BaudRate = 115200;//������Ϊ9600
    dcb1.ByteSize = 8;//ÿ���ֽ���8λ
    dcb1.Parity = NOPARITY;//����żУ��λ
    dcb1.StopBits = ONESTOPBIT;//����ֹͣλ
    dcb1.fParity = FALSE;
    dcb1.fNull = FALSE;
    SetCommState(hCom, &dcb1);

    DWORD wCount = 0;
    DWORD rCount = 0;

    PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);//��ջ�����

    int image_len = 0;
    unsigned char *image_buf = NULL;
    image_buf = (unsigned char *)malloc(40*1024);
    if(image_buf == NULL)
    {
        printf("�ڴ�����ʧ��\n");
        getchar();
        return FALSE;
    }
    memset(image_buf, 0, 40*1024);

    unsigned char str[300] = {0};

    WriteFile(hCom, imager_cmd, sizeof(imager_cmd), &wCount, NULL);
    if(!ReadFile(hCom, str, 6, &rCount, NULL))
    {
        printf("������ʧ��1!");
        getchar();
        return FALSE;
    }
    if(memcmp(str, ack_s, 6))
    {
        printf("ʧ��2\n");
        getchar();
        return FALSE;
    }

   Sleep(20);

    WriteFile(hCom, tig_cmd, sizeof(tig_cmd), &wCount, NULL);
    if(!ReadFile(hCom, str, 6, &rCount, NULL))
    {
        printf("������ʧ��3!");
        getchar();
        return FALSE;
    }
    if(memcmp(str, ack_s, 6))
    {
        printf("ʧ��4\n");
        getchar();
        return FALSE;
    }


    if(!ReadFile(hCom, str, 256, &rCount, NULL))
    {
        printf("������ʧ��5!");
        getchar();
        return FALSE;
    }
    if(rCount != 256)
    {
        printf("ʧ��6\n");
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
            printf("������ʧ��7!");
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
