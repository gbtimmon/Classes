#include "global.h"
#include "file.h"
#include "oper.h"

#include <fuse.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct fuse_operations gfs_oper = {
//     .access = gfs_access,
//     .bmap = gfs_bmap,
//     .chmod = gfs_chmod,
//     .chown = gfs_chown,
     .create = gfs_create,
//     .destroy = gfs_destroy,
//     .fallocate = gfs_fallocate,
//     .fgetattr = gfs_fgetattr,
//     .flock = gfs_flock,
//     .flush = gfs_flush,
//     .fsyncdir = gfs_fsyncdir,
//     .fsync = gfs_fsync,
//     .ftruncate = gfs_ftruncate,
     .getattr = gfs_getattr,
     .getdir = NULL,
//     .getxattr = gfs_getxattr,
//     .ioctl = gfs_ioctl,
//     .link = gfs_link,
//     .listxattra = gfs_listxattra,
//     .lock = gfs_lock,
     .mkdir = gfs_mkdir,
//     .mknod = gfs_mknod,
     .opendir = gfs_opendir,
     .open = gfs_open,
//     .poll = gfs_poll,
//     .read_buf = gfs_read_buf,
     .readdir = gfs_readdir,
     .read = gfs_read,
//     .readlink = gfs_readlink,
//     .releasedir = gfs_releasedir,
//     .release = gfs_release,
//     .removexattr = gfs_removexattr,
//     .rename = gfs_rename,
     .rmdir = gfs_rmdir,
//     .setxattr = gfs_setxattr,
//     .statfs = gfs_statfs,
//     .symlink = gfs_symlink,
     .truncate = gfs_truncate,
     .unlink = gfs_unlink,
//     .utime = gfs_utime,
//     .utimens = gfs_utimens,
//     .write_buf = gfs_write_buf,
     .write = gfs_write,

/**
 * Flag indicating that the filesystem can accept a NULL path
 * as the first argument for the following operations:
 *
 * read, write, flush, release, fsync, readdir, releasedir,
 * fsyncdir, ftruncate, fgetattr, lock, ioctl and poll
 *
 * If this flag is set these operations continue to work on
 * unlinked files even if "-ohard_remove" option was specified.
 */
     .flag_nullpath_ok = 1, 

/**
 * Flag indicating that the path need not be calculated for
 * the following operations:
 *
 * read, write, flush, release, fsync, readdir, releasedir,
 * fsyncdir, ftruncate, fgetattr, lock, ioctl and poll
 *
 * Closely related to flag_nullpath_ok, but if this flag is
 * set then the path will not be calculaged even if the file
 * wasn't unlinked.  However the path can still be non-NULL if
 * it needs to be calculated for some other reason.
 */

     .flag_nopath = 1,

/**
 * Flag indicating that the filesystem accepts special
 * UTIME_NOW and UTIME_OMIT values in its utimens operation.
 */
     .flag_utime_omit_ok = 1,

/**
 * Reserved flags, don't set
 */
     .flag_reserved = 29, 

};


int gfs_getattr (const char * path, struct stat * stbuf) 
{ 
    errno = 0;
    //Log_msg("gfs_getattr(path=\"%s\")\n", path);
    memset(stbuf, 0, sizeof(struct stat));

    File fattr = File_find( path ); 

    if( fattr == NULL ) {
        //Log_msg("\tError:File look up failed\n"); 
        //inhierit the failed lookup error. 
    }else {
        stbuf->st_mode = fattr->mode; 
        stbuf->st_nlink = 2;
        stbuf->st_size =  fattr->buf_sz;
        errno = 0; 
    } 
    //Log_msg("\t returning %d\n", -errno);
    return -errno; 
}

int gfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    errno = 0; 
    //Log_msg("gfs_readdir(path=\"%s\", buf=0x%08x, filler=0x%08x, offset=%lld, fi=0x%08x)\n", path, buf, filler, offset, fi); 

    File dir = (File) fi->fh; 
    if( dir == NULL ){ 
        //really shouldnt happen since already checke din dir open, but sanity check. 
        //Log_msg("\tError:Failed to find destination dir\n"); 
        errno = ENOENT; 

    } else if( !ISDIR(dir) ){ 
        //really shount happen since already checked in dir open, but sanity check. 
        //Log_msg("\tError:Not a dir\n"); 
        errno = ENOTDIR; 
    } else {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        File ele = dir->head;
        while( ele != NULL ) {
            filler(buf, ele->name, NULL, 0); 
            ele = ele->next; 
        }
    }
    return -errno;
}

int gfs_mkdir (const char * path, mode_t mode) {
    
    errno = 0; 
    //Log_msg("gfs_mkdir(path=\"%s\")\n", path);

    char* filename; 
    char* dirpath = File_dirname( path, &filename ); 
    
    if( dirpath == NULL ) return -errno;
    
    File  dir = File_find( dirpath ); 

    if( dir == NULL ) {
        //Log_msg("\tError:Failed to find destination dir\n"); 
        //inhierit the File find error. 
    } else {
        File_new_dir(dir, filename);
        //inhierit the new file errno; 
    }

    free(dirpath); 
    free(filename); 
    return -errno; 
}

int gfs_opendir(const char * path, struct fuse_file_info * fi ) {
    errno = 0; 
    //Log_msg("gfs_opendir(path=\"%s\")\n", path);

    File dir = File_find( path ) ; 

    if ( dir == NULL ) { 
       //Log_msg("\tError: File lookup failed\n"); 
       //inhierit the error of the failed find. 
       
    } else if( !ISDIR(dir) ) {
       //Log_msg("\tError:Not a directory\n");
       errno = ENOTDIR; 

    } else {
       fi->fh = (unsigned long ) dir; 
       errno = 0; 
    }

    return -errno; 
}

int gfs_rmdir (const char * path ) { 
    //Log_msg("gfs_rmdir(path=\"%s\")\n", path);

    File dir = File_find( path ) ;

    if ( dir == NULL ) {
       //Log_msg("\tError:File lookup failed\n");
       //inhierit the error of the failed find. 

    } else if ( !ISDIR(dir) ) { 
       //Log_msg("\tError:Not a directory\n");
       errno = ENOTDIR; 

    } else if( dir->head != NULL ) {
        //Log_msg("\tError:Cant delete non empty dir.\n"); 
        errno = EPERM;

    } else {

        if( dir->last != NULL )
            dir->last->next = dir->next;

        if( dir->next != NULL )  
            dir->next->last = dir->last; 

        if( dir->up->head == dir)
            dir->up->head = dir->next; 
        
        errno = 0; 
    }
    return -errno;
}

int gfs_create (const char * path, mode_t mode, struct fuse_file_info * fi ){

    errno = 0; 
    //Log_msg("gfs_create(path=\"%s\" mode=\"%d\", fi=\"%p\")\n", path, mode, (void*) fi); 
 
    char * filename; 
    char * dirpath = File_dirname( path, &filename ) ; 

    File dir = File_find( dirpath); 

    if ( dir == NULL ) {
        //Log_msg("\tError:File lookup failed\n");
        //inhierit the error of the failed find. 

    } else if ( !ISDIR(dir) ) { 
        //Log_msg("\tError:Not a directory\n");
        errno = ENOTDIR; 

    } else { 
        File newf = File_new( dir, filename ); 
        fi->fh = (unsigned long) newf; 
        //inhierit the newfile error;
    }

    free(filename); 
    free(dirpath); 
    return -errno ; 
}

int gfs_unlink (const char * path) {
    
    errno = 0;
    //Log_msg("gfs_unlink(path=\"%s\")\n"); 
   
    File node = File_find( path ); 

    if( node == NULL ) { 
        //Log_msg("\tError:File lookup failed\n");
        //inhierit the error of the failed find.

    } else if ( ISDIR(node) ) {
        //Log_msg("\tError:File is a directory\n"); 
        errno = EISDIR; 

    } else {
        current_size = current_size - node->buf_sz; 
        File_unlink( node );
        File_free( node ); 
        errno = 0; 
    }

    return -errno; 
}

int gfs_read (const char * path, char * buf, size_t sz, off_t off, struct fuse_file_info * fi ) 
{
    
    errno = 0; 
    //Log_msg("gfs_read(path=\"%s\" buf=\"%p\", sz=\"%d\" off=\"%d\" fi=\"%p\")\n", path, buf, sz, off, (void*) fi ); 

    File file = (File) fi->fh; 
    
    if( file == NULL ) {
        //Log_msg("Error:File lookup failed\n");
        errno = ENOENT; 

    }else if( ISDIR(file) ){
        //Log_msg("\tError:File is a directory\n");
        errno = EISDIR;

    }else if( off  > file->buf_sz )  {
        //Log_msg("\tError:Illegal Access\n"); 
        errno = ENFILE;

    } else {
        //Log_msg("I AM READING\n"); 
        int real_sz = ((sz + off) > file->buf_sz) ? file->buf_sz - off : sz ; 
   
        for( int i = 0; i < real_sz; i++ ) {
            buf[i] = file->buf[off + i];
        }
        //Log_msg("I am exiting\n");  
        return real_sz;

    }
    //Log_msg("I am exiting\n");
    return -errno; 
}

int gfs_write (const char * path, const char * buf, size_t sz, off_t off, struct fuse_file_info * fi ) {

    errno = 0; 
    //Log_msg("gfs_write(path=\"%s\" buf=\"%s\", sz=\"%d\" off=\"%d\" fi=\"%p\")\n", path, &buf, sz, off, (void*) fi); 

    File file = (File) fi->fh; 
    size_t end_of_write = off + sz; 
    size_t new_space    = end_of_write - file->buf_sz;

    if( file == NULL ) { 
        //Log_msg("\tError:File lookup failed\n");
        errno = ENOENT; 
        
    } else if( current_size + new_space > size_limit_bytes ) {
        //Log_msg("\tError:No more space\n");
        errno = ENOSPC;

    } else if( file->buf == NULL ) { 
        //Log_msg("\tMallocing a new buffer\n"); 


        file->buf    = calloc( sizeof(char), new_space ); 
        file->buf_sz = new_space;  
        current_size = current_size + new_space;
        
        if(file->buf == NULL ){
           return ENOMEM;
        }

    } else if( file->buf_sz < end_of_write) {
        //Log_msg("\tReallocing a larger file\n"); 
        file->buf    = realloc(file->buf, sizeof(char) * end_of_write); 
        file->buf_sz = end_of_write;
        current_size = current_size + new_space;

        if( file->buf == NULL ){
            return ENOMEM;
        }

        memset( file->buf + file->buf_sz, 0, end_of_write - file->buf_sz ); 

    } else {   
        //Log_msg("\tFile seems to be large enough"); 
    }
    
    if( errno == 0 ){
        for( int i = 0; i < sz; i++){
            file->buf[ off + i ] = buf[i]; 
        }
    }

    return (errno == 0 ) ? sz : errno; 
}

int gfs_open( const char * path, struct fuse_file_info * fi ) {

    errno = 0; 
    //Log_msg( "gfs_open(path=\"%s\" fi=\"%p\")\n", path, (void*) fi);

    File file = File_find( path ); 

    if ( file == NULL ) { 
       //Log_msg("\tError: File lookup failed\n"); 
       //inhierit the error of the failed find. 
       
    } else if( ISDIR(file) ) {
       //Log_msg("\tError:File is a directory  \n");
       errno = EISDIR; 

    }  else { 
        fi->fh = (unsigned long) file;
        errno = 0; 

    }
   
    return -errno; 
}


int gfs_truncate ( const char * path, off_t off ) { 
 
    errno = 0; 
    //Log_msg( "gfs_truncate(path=\"%s\" off=\"%d\")\n", path, off); 

    File file = File_find( path ); 

    if ( file == NULL ) {
        //Log_msg("\tError: File lookup failed\n");
        //inhierit the error of the failed find.
    } else if( ISDIR(file) ) {
        //Log_msg("\tError:File is a directory  \n");
        errno = EISDIR;

    }  else {
        file->buf    = realloc(file->buf, sizeof(char) * off );     

        long int change = file->buf_sz - off;
        current_size = current_size - change;        

        if( off > file->buf_sz ) 
            memset( file->buf + file->buf_sz, 0, sizeof(char) * (off - file->buf_sz) ); 

        file->buf_sz = off; 

        errno = 0; 
    }

    return -errno; 
}
//int    gfs_access      (const char *, int)
//int    gfs_bmap        (const char *, size_t blocksize, uint64_t *idx)
//int    gfs_chmod       (const char *, mode_t)
//int    gfs_chown       (const char *, uid_t, gid_t)
//int    gfs_fallocate   (const char *, int, off_t, off_t, struct fuse_file_info *)
//int    gfs_fgetattr    (const char *, struct stat *, struct fuse_file_info *)
//int    gfs_flock       (const char *, struct fuse_file_info *, int op)
//int    gfs_flush       (const char *, struct fuse_file_info *)
//int    gfs_fsync       (const char *, int, struct fuse_file_info *)
//int    gfs_fsyncdir    (const char *, int, struct fuse_file_info *)
//int    gfs_ftruncate   (const char *, off_t, struct fuse_file_info *)
 /* Deprecated, use readdir() instead */
//int    gfs_getdir      (const char *, fuse_dirh_t, fuse_dirfil_t)
//int    gfs_getxattr    (const char *, const char *, char *, size_t)
//int    gfs_ioctl       (const char *, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data)
//int    gfs_link        (const char *, const char *)
//int    gfs_listxattra  (const char *, char *, size_t)
//int    gfs_lock        (const char *, struct fuse_file_info *, int cmd, struct flock *)
//int    gfs_mknod       (const char *, mode_t, dev_t)
//int    gfs_open        (const char *, path , struct fuse_file_info * fi);
//int    gfs_poll        (const char *, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp)
//int    gfs_read_buf    (const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *)
//int    gfs_readlink    (const char *, char *, size_t)
//int    gfs_release     (const char *, struct fuse_file_info *)
//int    gfs_releasedir  (const char *, struct fuse_file_info *)
//int    gfs_removexattr (const char *, const char *)
//int    gfs_rename      (const char *, const char *)
//int    gfs_setxattr    (const char *, const char *, const char *, size_t, int)
//int    gfs_statfs      (const char *, struct statvfs *)
//int    gfs_symlink     (const char *, const char *)
//int    gfs_utime       (const char *, struct utimbuf *)
//int    gfs_utimens     (const char *, const struct timespec tv[2])
//int    gfs_write_buf   (const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *)
//void   gfs_destroy     (void *)
//void * gfs_init        (struct fuse_conn_info *conn)
