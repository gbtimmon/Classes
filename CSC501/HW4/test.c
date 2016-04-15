#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <unistd.h>
#include <fuse.h>

#include "operations.h"

int main(int argc, char *argv[])
{
    int fuse_stat = -1;

    if ((getuid() == 0) || (geteuid() == 0)) {
	fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
	return 1;
    }

    fprintf(stdout, "yo dog nignog\n");   
    fuse_stat = fuse_main(argc, argv, &gfs_oper, NULL);
    fprintf(stdout, "yo dog\n");
    
    return fuse_stat;
}
