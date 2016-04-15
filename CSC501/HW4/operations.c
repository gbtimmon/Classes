
#include "params.h"
#include "operations.h"

#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>

static const char *gfs_str = "Hello World!\n";
static const char *gfs_path = "/hello";

int gfs_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, gfs_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(gfs_str);
	} else
		res = -ENOENT;

	return res;
}

int gfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, gfs_path + 1, NULL, 0);

	return 0;
}

int gfs_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, gfs_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

int gfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, gfs_path) != 0)
		return -ENOENT;

	len = strlen(gfs_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, gfs_str + offset, size);
	} else
		size = 0;

	return size;
}

int gfs_mkdir (const char * path, mode_t mode){
    printf("bitch I aint done yet!\n");
    return -1; 
}
