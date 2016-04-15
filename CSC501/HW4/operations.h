#ifndef _OPERATIONS_H
#define _OPERATIONS_H

#include <fuse.h>

int gfs_getattr(const char *path, struct stat *stbuf);
int gfs_open(const char * path, struct fuse_file_info *fi);
int gfs_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info *fi);
int gfs_write(const char * path, const char * buf, size_t size, off_t offset, struct fuse_file_info *fi);
int gfs_mkdir(const char * path, mode_t mode);
int gfs_unlink(const char * path);
int gfs_rmdir(const char * path);
int gfs_opendir(const char * path, struct fuse_file_info *fi);
int gfs_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);

static struct fuse_operations gfs_oper = {
        .readdir        = gfs_readdir,
        .open           = gfs_open,
        .read           = gfs_read,
        .write          = gfs_write,
        .opendir        = gfs_opendir,
        .mkdir          = gfs_mkdir,
        .unlink         = gfs_unlink,
        .rmdir          = gfs_rmdir,
        .getattr        = gfs_getattr,
};

#endif
