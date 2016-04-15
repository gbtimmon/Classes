#ifndef _OPERATIONS_C
#define _OPERATIONS_C


#include <fuse.h>
#include <stdio.h>

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

#endif
