#ifndef FILE_H
#define FILE_H

#define FILE_TYPE_FILE 0
#define FILE_TYPE_DIR  1 
#define FILE_TYPE_ROOT 2

#include <stdlib.h>

typedef struct fs_file { 
    int type; 
    char * name; 
    struct fs_file * up;
    struct fs_file * next; 
    struct fs_file * last; 
    struct fs_file * head; 
    void * buf; 
    size_t sz; 
    mode_t mode; 
} * File;


char* File_dirname( const char * ); 
File  File_find( const char * ); 
File  File_new_dir( File , const char * ); 
File  File_new_root(); 
#endif
