#ifndef FILE_C
#define FILE_C

#include "log.h"
#include "file.h"

#include <errno.h>
#include <stdlib.h>


File File_new_root( ) { 

     File root = malloc( sizeof( struct fs_file ) ); 

     root->type = FILE_TYPE_DIR; 
     root->up   = NULL;
     root->next = NULL;
     root->last = NULL;
     root->head = NULL;
     root->buf  = NULL;
  
     root->sz   = sizeof(struct fs_file ); 

}
File File_new_dir( File parent, const char * name ) { 

    if ( parent->type != FILE_TYPE_DIR ) {
        Log_msg("Error: Tried to create file in non-directory object\n");
        errno = ENOTDIR;   
        return NULL;
    }

    
}

#endif
