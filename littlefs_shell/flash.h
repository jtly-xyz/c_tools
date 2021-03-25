/*
 * littlefs_hal.h
 *
 *      Author: swc
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "littlefs/lfs.h"

#define LFS_FILE lfs_file_t*

void close_com();

int init_lfs();
lfs_file_t * file_open(const char * path, int flags);
lfs_ssize_t file_read(lfs_file_t *file, void *buffer, lfs_size_t size);
lfs_ssize_t file_write(lfs_file_t *file, void *buffer, lfs_size_t size);
int file_close(lfs_file_t *file);
int file_rewind(lfs_file_t *file);

int file_remove(const char *path);
int file_rename(const char *oldpath, const char *newpath);
int file_stat(const char *path, struct lfs_info *info);

int dir_mkdir(const char *path);
int dir_close(lfs_dir_t *dir);
int dir_read(lfs_dir_t *dir, struct lfs_info *info);
int dir_seek(lfs_dir_t *dir, lfs_off_t off);
int dir_rewind(lfs_dir_t *dir);

int file_setattr(const char *path, uint8_t type, const void *buffer, lfs_size_t size);
lfs_ssize_t file_getattr(const char *path, uint8_t type, void *buffer, lfs_size_t size);

#endif /* LITTLEFS_HAL_H_ */

