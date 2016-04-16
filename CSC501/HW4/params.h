#ifndef _PARAMS_H
#define _PARAMS_H

#define _FILE_OFFSET_BITS 64
#define _GNU_SOURCE
#define FUSE_USE_VERSION 26

#define BB_DATA ((struct gfs_state *) fuse_get_context()->private_data)

#include <limits.h>
#include <stdio.h>
struct gfs_state {
    FILE *logfile;
    char *rootdir;
};

#endif
