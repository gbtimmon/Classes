#ifndef FILE_H
#define FILE_H

#define FILE_TYPE_FILE 0
#define FILE_TYPE_DIR  1 
#define FILE_TYPE_ROOT 2

#define ISDIR(x) ( x->type == FILE_TYPE_DIR || x->type == FILE_TYPE_ROOT)

#include <stdlib.h>

typedef struct fs_file { 
    int type; 
    char * name; 
    struct fs_file * up;
    struct fs_file * next; 
    struct fs_file * last; 
    struct fs_file * head; 
    int    buf_sz; 
    char * buf; 
    mode_t mode; 
} * File;


char* File_dirname( const char *, char ** ); 
File  File_find( const char * ); 
File  File_new_dir( File , const char * ); 
File  File_new( File, const char * ); 
File  File_new_root(); 

int   File_unlink( File ); 
int   File_free( File ); 
#endif
