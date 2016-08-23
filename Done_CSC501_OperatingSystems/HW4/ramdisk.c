#include "global.h"
#include "file.h"
#include "oper.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fuse_stat = -1;
    State fsState = State_new("./greg_fs.log");

    if( argc != 3 ) { 
        printf("RAMDISK expects 2 parameters\n"); 
        exit (1); 
    }

    size_limit = atoi(argv[2]);    
    size_limit_bytes = size_limit * 1024 * 1024;
    current_size = 0; 
  
    argc = 2;
    argv[2] = NULL;

    fuse_stat = fuse_main(argc, argv, &gfs_oper, fsState );
    return fuse_stat;
}

