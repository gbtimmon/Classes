#include "global.h"
#include "file.h"
#include "oper.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    int fuse_stat = -1;
    State fsState = State_new("./greg_fs.log");

    if( argc != 3 ) { 
        printf("RAMDISK expects 2 parameters\n"); 
    }

    int size_limit = atoi(argv[2]);    
    int size_limit_bytes = size_limit * 1024 * 1024;
    
    struct rlimit rl = {size_limit_bytes, size_limit_bytes};
   
    setrlimit(RLIMIT_DATA, &rl); 
  
   

    fuse_stat = fuse_main(argc, argv, &gfs_oper, fsState );
    Log_msg("Testing the log %s\n", "AHHHH");
    return fuse_stat;
}

