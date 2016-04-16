#ifndef FILE_C
#define FILE_C

#include "global.h"
#include "file.h"

#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * copyString( const char * os, size_t * size){

    *size = sizeof(char) * strlen(os);

    char * ns = malloc( *size ); 
    strcpy(ns, os); 
    return ns; 

}
File File_new_root( ) { 

    File root = malloc( sizeof( struct fs_file ) ); 

    root->type = FILE_TYPE_ROOT; 
    root->name = "root";   
    root->up   = NULL;
    root->next = NULL;
    root->last = NULL;
    root->head = NULL;
    root->buf  = NULL;
  
    root->sz   = sizeof(struct fs_file ); 

    return root;
}

File File_new_dir( File parent, const char * name ) { 

    if ( parent->type != FILE_TYPE_DIR ) {
        Log_msg("Error: Tried to create file in non-directory object\n");
        errno = ENOTDIR;   
        return NULL;
    }

    //create new file. 
    size_t name_size; 
    File dir  = malloc( sizeof( struct fs_file ) );
    dir->type = FILE_TYPE_DIR;
    dir->name = copyString(name, &name_size); 
    dir->up   = parent;
    dir->mode = S_IFDIR | 0755;
    dir->next = NULL;
    dir->last = NULL;
    dir->head = NULL;
    dir->buf  = NULL;
    dir->sz   = sizeof(struct fs_file ) + name_size;

    //attach the file.
    File node = parent->head; 
    parent->head = dir; 
    dir->next    = node; 
    node->last   = dir; 
    return dir;
}

#endif
