#include "params.h"
#include "oper.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fuse_stat = -1;
    struct gfs_state * gfs_data = malloc(sizeof(struct gfs_state)); 
    gfs_data->logfile = log_open();
    

    if ((getuid() == 0) || (geteuid() == 0)) {
	fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
	return 1;
    }

    fuse_stat = fuse_main(argc, argv, &gfs_oper, gfs_data );
    log_msg("Testing the log %s\n", "AHHHH");
    return fuse_stat;
}

