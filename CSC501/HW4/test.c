#define _GNU_SOURCE
#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <unistd.h>
#include <fuse.h>

typedef struct fuse_operations* Oper;
Oper operations; 

int main(int argc, char *argv[])
{
    int fuse_stat = -1;

    if ((getuid() == 0) || (geteuid() == 0)) {
	fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
	return 1;
    }

    
    //fuse_stat = fuse_main(argc, argv, &bb_oper, bb_data);
    fprintf(stdout, "yo dog\n");
    
    return fuse_stat;
}
