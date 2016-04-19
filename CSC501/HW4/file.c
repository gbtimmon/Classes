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

    size_t sz = sizeof(char) * strlen(os);

    char * ns = malloc( sz ); 
    strcpy(ns, os); 

    if( size != NULL ) 
        *size = sz; 

    return ns; 
}

File File_find( const char * path ) {
   
    //Log_msg("\tFILE: Searching for %s\n", path); 
     
    if( strcmp( path, "(null)" ) == 0 || strcmp( path, "/" ) == 0 ) { 
        //Log_msg("\tFILE : Found file name %s with root path\n", getState()->root );
        errno = 0; 
        return getState()->root;
    }

    errno = 0; 
    File current  = getState()->root->head;
    char * string = copyString(path, NULL); 
    char * start  = string + 1; //all of my non root file start with / 
    char * end    = string + 1; //I can ignore that first / and root "/" only
    int end_found = 0;          //wont mkae it here. 


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
        //Log_msg("\tFILE: Searching for token %s\n", start); 

        while( 1 ) {

            if( current == NULL ) {
                Log_msg("\tFILE: File not Found\n"); 
                free(string); 
                errno = ENOENT; 
                return NULL;
            }

            if( strcmp(start, current->name ) == 0 ) { 
                
                if(end_found) {
                    free(string); 
                    //Log_msg("\tFILE : Found file name %s in %s\n", current->name, (current->up)? current->up->name : "NULL"); 
                    errno = 0; 
                    return current;
                } else {
                    if( ! ISDIR(current) ) {
                        //Log_msg("\tFILE: Trying to search file. Fatal \n");
                        errno = ENOTDIR;
                        return NULL;
                    } else {
                        //Log_msg("\tFILE: Dir found, going down. \n");
                        current = current->head; 
                        break; 
                    }
                }
            } else {
                current = current->next; 
            }
        }
    }
}

char * File_dirname( const char * in, char ** filename ) {
    int i    = 0;
    int last = -1;

    while(1) {
        if( in[i] == '/' ) {
            if( in[ i+1 ] == '\0' ){
                if( i == 0 ) {
                    last = 0;
                }
                break;
            }
            last = i;
        } else if ( in[i] == '\0' ) {
            break;
        }
        i++;
    }

    if( last < 1 ) {
        char * dir = malloc( sizeof(char) * 2 );
        strcpy( dir,( last == -1 ) ? ".": "/" );

        if( filename ) {
            char * fil = malloc( sizeof(char) * strlen(in) );
            strcpy( fil, in + ((last == -1 ) ? 0 : 1) );
            *filename = fil;

            while( *fil != '\0' ) {
                if( *fil == '/' ) *fil = '\0';
                fil++;
            }
        }

        return dir;

    }
    char * dir = malloc( ( sizeof(char) * last ) +  1 ) ;
    strncpy( dir, in, last);

    if( filename ) {
        char * fil = malloc( sizeof(char) * strlen( in + last ) );
        strcpy( fil, in + last + ((last == -1 ) ? 0 : 1) );
        *filename = fil;

        while( *fil != '\0' ) {
            if( *fil == '/' ) *fil = '\0';
            fil++;
        }
    }

    return dir;

}

File File_new_root( ) { 

    File root = malloc( sizeof( struct fs_file ) ); 

    root->type = FILE_TYPE_ROOT; 
    root->mode = S_IFDIR | 0755;
    root->name = "/";   
    root->up   = NULL;
    root->next = NULL;
    root->last = NULL;
    root->head = NULL;
    root->buf  = NULL;
  
    root->sz   = sizeof(struct fs_file ); 

    return root;
}

File File_new_dir( File parent, const char * name ) { 

    if ( ! ISDIR(parent) ) {
        Log_msg("Error: Tried to create file in non-directory object\n");
        errno = ENOTDIR;   
        return NULL;
    }

    //create new file. 
    size_t name_size; 
    File dir  = malloc( sizeof( struct fs_file ) );
    dir->type = FILE_TYPE_DIR;
    dir->name = copyString( name, &name_size ); 
    dir->up   = parent;
    dir->mode = S_IFDIR | 0755;
    dir->next = NULL;
    dir->last = NULL;
    dir->head = NULL;
    dir->buf  = NULL;
    dir->sz   = sizeof(struct fs_file ) + name_size;

    //attach the file.
    if( parent->head )
        parent->head->last = dir; 

    dir->next    = parent->head; 
    parent->head = dir; 

    return dir;
}

File File_new( File dir, const char * name  ) { 

    if ( ! ISDIR(dir) ) {
        Log_msg("Error: Tried to create file in non-directory object\n");
        errno = ENOTDIR;   
        return NULL;
    }

    size_t name_size; 

    File node = malloc( sizeof( struct fs_file ) ); 
    node->type = FILE_TYPE_FILE; 
    node->name = copyString( name, &name_size ); 
    node->up   = dir; 
    node->mode = S_IFREG | 0755; 
    node->next = NULL; 
    node->last = NULL; 
    node->head = NULL; 
    node->buf  = NULL;
    node->sz   = sizeof(struct fs_file ) + name_size;

    //ATTACH THE FILE 
    if( dir->head )  
        dir->head->last = node; 

    node->next = dir->head; 
    dir->head  = node; 

    return node; 
}

int File_unlink( File node ) { 
    
    if( node->next ) node->next->last = node->last; 
    if( node->last ) node->last->next = node->next; 
    if( node->up->head == node) node->up->head = node->next; 

    return 0; 
}

int File_free( File node ) {

    if( node ) free(node->buf); 
    if( node ) free(node->name); 
    free(node); 
    
    return 0; 
} 

#endif
