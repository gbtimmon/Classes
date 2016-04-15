#ifndef _OPERATIONS_C
#define _OPERATIONS_C


#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int gfs_open(const char * path, struct fuse_file_info *fi){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info *fi){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_write(const char * path, const char * buf, size_t size, off_t offset, struct fuse_file_info *fi){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_mkdir(const char * path, mode_t mode){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_unlink(const char * path){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_rmdir(const char * path){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_opendir(const char * path, struct fuse_file_info *fi){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}
int gfs_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
    printf("Dude I aint dont this shit.... fuck off\n");
    return -1; 
}

int gfs_getattr(const char *path, struct stat *stbuf)
{
        int res = 0;

        memset(stbuf, 0, sizeof(struct stat));
        if (strcmp(path, "/") == 0) {
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;
  //      } else if (strcmp(path, hello_path) == 0) {
 //              stbuf->st_mode = S_IFREG | 0444;
  //              stbuf->st_nlink = 1;
   //             stbuf->st_size = strlen(hello_str);
        } else
                res = -ENOENT;

        return res;
}


#endif
