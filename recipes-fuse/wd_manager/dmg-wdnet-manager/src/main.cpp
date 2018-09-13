
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>


#include "fuse_operations_interface.h"
#include "tef_fuse_operations.h"

using namespace std;

FuseOperationsInterface* g_fuse_class;


static int power_getattr(const char *path, struct stat *stbuf)
{
    return g_fuse_class->power_getattr(path,stbuf);
}

static int power_readDir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    return g_fuse_class->power_readDir(path,buf,filler,offset,fi);
}

static int power_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    return g_fuse_class->power_read(path,buf,size,offset,fi);
}

static int power_write(const char *path, const char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
    return g_fuse_class->power_write(path,buf,size,offset,fi);
}

static int power_dummy_statfs (const char *a, struct statvfs *b)
{
   return g_fuse_class->power_dummy_statfs(a,b);
}

int power_dummy_setxattr(const char *a, const char *b, const char *c, size_t d, int x)
{
    return g_fuse_class->power_dummy_setxattr(a,b,c,d,x);
}

static int power_dummy_reloc(const char *a, off_t b)
{
    return g_fuse_class->power_dummy_reloc(a,b);
}

static int power_dummy_flush(const char *a, struct fuse_file_info *b)
{
    return g_fuse_class->power_dummy_flush(a,b);
}

static int power_dummy_getxattr(const char *a, const char *b, char *c, size_t d)
{
    return g_fuse_class->power_dummy_getxattr(a,b,c,d);
}

void *power_init_fs (struct fuse_conn_info *conn)
{
    return g_fuse_class->power_init_fs(conn);
}

struct FuseOperations: public fuse_operations
{
  FuseOperations()
  {
  init     = power_init_fs;
  getattr  = power_getattr;
  readdir  = power_readDir;
  read     = power_read;
  write    = power_write;
  statfs   = power_dummy_statfs;
  setxattr = power_dummy_setxattr;
  getxattr = power_dummy_getxattr;
  truncate = power_dummy_reloc;
  flush    = power_dummy_flush;
  }
};

static const FuseOperations tefOperations;

int TEFMain(int argc, char *argv[])
{
   g_fuse_class=new TefFuseOperations();
    return fuse_main(argc-1, argv, &tefOperations, NULL);
}





int main(int argc, char *argv[])
{

    TEFMain(argc,argv);
    return 0;
}
