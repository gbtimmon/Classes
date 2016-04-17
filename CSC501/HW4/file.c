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

    Log_msg("copy sstirng\n");
    size_t sz = sizeof(char) * strlen(os);

    char * ns = malloc( sz ); 
    strcpy(ns, os); 

    if( size != NULL ) 
        *size = sz; 

    return ns; 
}

File File_find( const char * path ) {
     
    File current  = getState()->root->head;
    char * string = copyString(path, NULL); 
    char * start  = string; 
    char * end    = string;
    int end_found = 0;

    while(1) {
        start = end;

        while(1) {
            if ( *end == '/' ) {
                *end = '\0';
                end++;
                break;

            } else if( *end == '\0' ){
               end_found = 1;
               end++;
               break;

            } else {
                end++;

            }
        }

        while( 1 ) {

            if( current == NULL ) {
                free(string); 
                errno = ENOENT; 
                return NULL;
            }

            if( strcmp(start, current->name ) == 0 ) { 

                if(end_found) {
                    free(string); 
                    return current;
                } else {
                    if( current->type != FILE_TYPE_DIR 
                     || current->type != FILE_TYPE_ROOT ) {
                        errno = ENOTDIR;
                        return NULL;
                    }
                    break; 
                }
            } else {
                current = current->next; 
            }
        }
    }
}

char * File_dirname( const char * in ) {

    int i    = 0;
    int last = -1;

    while(1) {
        if( in[i] == '/' ) {
            if( in[ i+1 ] == '\0' )
                break;
            last = i;
        } else if ( in[i] == '\0' ) {
            break;
        }
        i++;
    }

    if( last == -1 ) { 
        char * out = malloc( sizeof(char) * 2 ); 
        strncpy( out, ".", 1); 
        return out; 
    }

    char * out = malloc( ( sizeof(char) * last ) + 1 ) ;
    strncpy( out, in, last);
    return out;

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

    if ( parent->type != FILE_TYPE_DIR && parent->type != FILE_TYPE_ROOT ) {
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
    if( node != NULL ) 
        node->last = dir; 

    return dir;
}

#endif
