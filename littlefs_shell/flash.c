#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flash.h"
#include <io.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include "tchar.h"

HANDLE hCom = NULL;

int init_com()
{
    hCom = CreateFile(_T("COM5"),
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
        return -1;
    }
    else
    {
        printf("COM打开成功！\n");
    }
    SetupComm(hCom, 10240, 10240);//输入缓冲区和输出缓冲区的大小都是1024

    COMMTIMEOUTS TimeOuts;//设定读超时
    TimeOuts.ReadIntervalTimeout = 10;
    TimeOuts.ReadTotalTimeoutMultiplier = 10;
    TimeOuts.ReadTotalTimeoutConstant = 5000;
    TimeOuts.WriteTotalTimeoutMultiplier = 50;//设定写超时
    TimeOuts.WriteTotalTimeoutConstant = 5000;
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


    PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);//清空缓冲区

    return 0;
}

void close_com()
{
    CloseHandle(hCom);
}


enum FLASH_CMD
{
    FLASH_READ = 0,
    FLASH_WRITE = 1,
    FLASH_ERASE = 2,
    FLASH_ACK = 3,
};



int littlefs_W25QXX_Read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    DWORD wCount = 0;
    DWORD rCount = 0;
    static unsigned char cmd_buf[] = {'L', 'F', 'S', FLASH_READ, 0, 0, 0, 0, 0, 0, '\n'};
    unsigned int * tmp1 = (unsigned int *)(&(cmd_buf[4]));
    unsigned short * tmp2 = (unsigned short *)(&(cmd_buf[8]));
    *tmp1 = block * c->block_size + off;
    *tmp2 = (unsigned short)size;

    printf("R:%d %d\n", *tmp1, *tmp2);

//	W25QXX_Read(buffer, block * c->block_size + off, size);
    WriteFile(hCom, cmd_buf, sizeof(cmd_buf), &wCount, NULL);
    if(FALSE == ReadFile(hCom, buffer, size, &rCount, NULL))
    {
        return  LFS_ERR_IO;
    }
    else

	return LFS_ERR_OK;
}

//LFS CMD ADDR size \n
//3    1   4     2   1
int littlefs_W25QXX_Write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    DWORD wCount = 0;
    DWORD rCount = 0;
    unsigned char read_buf[4] = {0};
    static unsigned char cmd_buf[] = {'L', 'F', 'S', FLASH_WRITE, 0, 0, 0, 0, 0, 0, '\n'};
    unsigned int * tmp1 = (unsigned int *)(&(cmd_buf[4]));
    unsigned short * tmp2 = (unsigned short *)(&(cmd_buf[8]));
    *tmp1 = block * c->block_size + off;
    *tmp2 = (unsigned short)size;

    printf("W:%d %d\n", *tmp1, *tmp2);

    WriteFile(hCom, cmd_buf, sizeof(cmd_buf), &wCount, NULL);
    if(FALSE == ReadFile(hCom, read_buf, 4, &rCount, NULL))
    {
        return  LFS_ERR_IO;
    }
    else
    {
        if((read_buf[0] == 'L') && (read_buf[1] == 'F') && (read_buf[2] == 'S') && (read_buf[3] == FLASH_ACK))
        {
            WriteFile(hCom, buffer, size, &wCount, NULL);
            if(FALSE == ReadFile(hCom, read_buf, 4, &rCount, NULL))
            {
                return  LFS_ERR_IO;
            }
            else
            {
                if((read_buf[0] == 'L') && (read_buf[1] == 'F') && (read_buf[2] == 'S') && (read_buf[3] == FLASH_ACK))
                {
                }
                else
                {
                    return  LFS_ERR_IO;
                }
            }

        }
        else
        {
            return  LFS_ERR_IO;
        }
    }

//	W25QXX_Write(buffer, block * c->block_size + off, size);

	return LFS_ERR_OK;
}

int littlefs_W25QXX_Erase_Sector(const struct lfs_config *c, lfs_block_t block)
{
    DWORD wCount = 0;
    DWORD rCount = 0;
    unsigned char read_buf[4] = {0};
    static unsigned char cmd_buf[] = {'L', 'F', 'S', FLASH_ERASE, 0, 0, 0, 0, 0, 0, '\n'};
    unsigned int * tmp1 = (unsigned int *)(&(cmd_buf[4]));
    *tmp1 = block;

    WriteFile(hCom, cmd_buf, sizeof(cmd_buf), &wCount, NULL);
    if(FALSE == ReadFile(hCom, read_buf, 4, &rCount, NULL))
    {
        return  LFS_ERR_IO;
    }
    else
    {
        if((read_buf[0] == 'L') && (read_buf[1] == 'F') && (read_buf[2] == 'S') && (read_buf[3] == FLASH_ACK))
        {

        }
        else
        {
            return  LFS_ERR_IO;
        }
    }

//	W25QXX_Erase_Sector(block);

	return LFS_ERR_OK;
}

int littlefs_W25QXX_Sync(const struct lfs_config *c)
{
	return LFS_ERR_OK;
}


static lfs_t lfs;

static const struct lfs_config cfg = {
    // block device operations
    .read  = littlefs_W25QXX_Read,
    .prog  = littlefs_W25QXX_Write,
    .erase = littlefs_W25QXX_Erase_Sector,
    .sync  = littlefs_W25QXX_Sync,

    // block device configuration
    .read_size = 1,
    .prog_size = 1,
    .block_size = 4096,
    .block_count = 8192,
    .cache_size = 512,
    .lookahead_size = 16,
    .block_cycles = 500,
};

int init_lfs()
{
	int err = 0;
	if(init_com() != 0)
	{
		return -1;
	}

//	lfs_format(&lfs, &cfg);
	err = lfs_mount(&lfs, &cfg);
	if(err)
	{
	    return -1;
//		lfs_format(&lfs, &cfg);
//		if(lfs_mount(&lfs, &cfg))
//		{
//		    close_com();
//			return -1;
//		}
	}

	return 0;
}


lfs_file_t * file_open(const char * path, int flags)
{
	lfs_file_t * file = (lfs_file_t * )malloc(sizeof(lfs_file_t));

	if(file == NULL)
		return NULL;

	if(lfs_file_open(&lfs, file, path, flags) != LFS_ERR_OK)
    {
        free(file);

        return NULL;
    }


	return file;
}

lfs_ssize_t file_read(lfs_file_t *file, void *buffer, lfs_size_t size)
{
	return lfs_file_read(&lfs, file, buffer, size);
}

lfs_ssize_t file_write(lfs_file_t *file, void *buffer, lfs_size_t size)
{
	return lfs_file_write(&lfs, file, buffer, size);
}

int file_close(lfs_file_t *file)
{
	if(file == NULL)
		return LFS_ERR_IO;

	lfs_file_close(&lfs, file);

	free(file);

	return LFS_ERR_OK;
}

int file_rewind(lfs_file_t *file)
{
	return lfs_file_rewind(&lfs, file);
}

int file_remove(const char *path)
{
    return lfs_remove(&lfs, path);
}
int file_rename(const char *oldpath, const char *newpath)
{
    return lfs_rename(&lfs, oldpath, newpath);
}
int file_stat(const char *path, struct lfs_info *info)
{
    return lfs_stat(&lfs, path, info);
}


int dir_mkdir(const char *path)
{
    return lfs_mkdir(&lfs, path);
}
lfs_dir_t * dir_open(const char *path)
{
    lfs_dir_t * dir = (lfs_dir_t *)malloc(sizeof(lfs_dir_t));

    if(dir == NULL)
		return NULL;

    if(lfs_dir_open(&lfs, dir, path) != LFS_ERR_OK)
    {
        free(dir);
        return NULL;
    }

    return dir;
}

int dir_close(lfs_dir_t *dir)
{
    if(dir == NULL)
		return LFS_ERR_IO;

	lfs_dir_close(&lfs, dir);

	free(dir);

	return LFS_ERR_OK;
}
int dir_read(lfs_dir_t *dir, struct lfs_info *info)
{
     return  lfs_dir_read(&lfs, dir, info);
}
int dir_seek(lfs_dir_t *dir, lfs_off_t off)
{
     return lfs_dir_seek(&lfs, dir, off);
}
int dir_rewind(lfs_dir_t *dir)
{
     return lfs_dir_rewind(&lfs, dir);
}

int file_setattr(const char *path, uint8_t type, const void *buffer, lfs_size_t size)
{
    return lfs_setattr(&lfs, path, type, buffer, size);
}

lfs_ssize_t file_getattr(const char *path, uint8_t type, void *buffer, lfs_size_t size)
{
    return lfs_getattr(&lfs, path, type, buffer, size);
}
