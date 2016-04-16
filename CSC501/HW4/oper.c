#include "params.h"
#include <fuse.h>

struct fuse_operations gfs_oper = {
     .getattr = gfs_getattr,
//     .readlink = gfs_readlink,
//     .getdir = gfs_getdir,
//     .mknod = gfs_mknod,
//     .mkdir = gfs_mkdir,
//     .unlink = gfs_unlink,
//     .rmdir = gfs_rmdir,
//     .symlink = gfs_symlink,
//     .rename = gfs_rename,
//     .link = gfs_link,
//     .chmod = gfs_chmod,
//     .chown = gfs_chown,
//     .truncate = gfs_truncate,
//     .utime = gfs_utime,
//     .open = gfs_open,
//     .read = gfs_read,
//     .write = gfs_write,
//     .statfs = gfs_statfs,
//     .flush = gfs_flush,
//     .release = gfs_release,
//     .fsync = gfs_fsync,
//     .setxattr = gfs_setxattr,
//     .getxattr = gfs_getxattr,
//     .listxattra = gfs_listxattra,
//     .removexattr = gfs_removexattr,
//     .opendir = gfs_opendir,
//     .readdir = gfs_readdir,
//     .releasedir = gfs_releasedir,
//     .fsyncdir = gfs_fsyncdir,
//     .destroy = gfs_destroy,
//     .access = gfs_access,
//     .create = gfs_create,
//     .ftruncate = gfs_ftruncate,
//     .fgetattr = gfs_fgetattr,
//     .lock = gfs_lock,
//     .utimens = gfs_utimens,
//     .bmap = gfs_bmap,
//     .ioctl = gfs_ioctl,
//     .poll = gfs_poll,
//     .write_buf = gfs_write_buf,
//     .read_buf = gfs_read_buf,
//     .flock = gfs_flock,
//     .fallocate = gfs_fallocate,
};


unsigned int flag_nullpath_ok:1;
unsigned int flag_nopath:1;
unsigned int flag_utime_omit_ok:1;
unsigned int flag_reserved:29;

//int    gfs_getattr     (const char *, struct stat *)
//int    gfs_readlink    (const char *, char *, size_t)
///* Deprecated, use readdir() instead */
//int    gfs_getdir      (const char *, fuse_dirh_t, fuse_dirfil_t)
//int    gfs_mknod       (const char *, mode_t, dev_t)
//int    gfs_mkdir       (const char *, mode_t)
//int    gfs_unlink      (const char *)
//int    gfs_rmdir       (const char *)
//int    gfs_symlink     (const char *, const char *)
//int    gfs_rename      (const char *, const char *)
//int    gfs_link        (const char *, const char *)
//int    gfs_chmod       (const char *, mode_t)
//int    gfs_chown       (const char *, uid_t, gid_t)
//int    gfs_truncate    (const char *, off_t)
//int    gfs_utime       (const char *, struct utimbuf *)
//int    gfs_open        (const char *, struct fuse_file_info *)
//int    gfs_read        (const char *, char *, size_t, off_t, struct fuse_file_info *)
//int    gfs_write       (const char *, const char *, size_t, off_t, struct fuse_file_info *)
//int    gfs_statfs      (const char *, struct statvfs *)
//int    gfs_flush       (const char *, struct fuse_file_info *)
//int    gfs_release     (const char *, struct fuse_file_info *)
//int    gfs_fsync       (const char *, int, struct fuse_file_info *)
//int    gfs_setxattr    (const char *, const char *, const char *, size_t, int)
//int    gfs_getxattr    (const char *, const char *, char *, size_t)
//int    gfs_listxattra  (const char *, char *, size_t)
//int    gfs_removexattr (const char *, const char *)
//int    gfs_opendir     (const char *, struct fuse_file_info *)
//int    gfs_readdir     (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *)
//int    gfs_releasedir  (const char *, struct fuse_file_info *)
//int    gfs_fsyncdir    (const char *, int, struct fuse_file_info *)
//void * gfs_init        (struct fuse_conn_info *conn)
//void   gfs_destroy     (void *)
//int    gfs_access      (const char *, int)
//int    gfs_create      (const char *, mode_t, struct fuse_file_info *)
//int    gfs_ftruncate   (const char *, off_t, struct fuse_file_info *)
//int    gfs_fgetattr    (const char *, struct stat *, struct fuse_file_info *)
//int    gfs_lock        (const char *, struct fuse_file_info *, int cmd, struct flock *)
//int    gfs_utimens     (const char *, const struct timespec tv[2])
//int    gfs_bmap        (const char *, size_t blocksize, uint64_t *idx)
//int    gfs_ioctl       (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data)
//int    gfs_poll        (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp)
//int    gfs_write_buf   (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *)
//int    gfs_read_buf    (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *)
//int    gfs_flock       (const char *, struct fuse_file_info *, int op)
//int    gfs_fallocate   (const char *, int, off_t, off_t, struct fuse_file_info *)
