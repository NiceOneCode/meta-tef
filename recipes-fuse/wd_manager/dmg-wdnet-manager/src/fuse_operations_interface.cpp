#include "fuse_operations_interface.h"

int FuseOperationsInterface::power_getattr(const char *path, struct stat *stbuf)
{
    (void) path;
    (void) stbuf;
    return 0;
}

int FuseOperationsInterface::power_readDir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi)
{
    (void) path;
    (void) offset;
    (void) buf;
    (void) filler;
    (void) fi;
    return 0;
}
int FuseOperationsInterface::power_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    (void) path;
    (void) buf;
    (void) size;
    (void) offset;
    (void) fi;
    return 0;
}
int FuseOperationsInterface::power_write(const char *path, const char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
    (void) path;
    (void) buf;
    (void) size;
    (void) offset;
    (void) fi;
    return 0;
}

int FuseOperationsInterface::power_dummy_statfs (const char *a, struct statvfs *b)
{
  (void) a;
  (void) b;
  return 0;
}

int FuseOperationsInterface::power_dummy_setxattr(const char *a, const char *b, const char *c, size_t d, int x)
{
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) x;
  return 0;
}

int FuseOperationsInterface::power_dummy_reloc(const char *a, off_t b)
{
  (void) a;
  (void) b;
  return 0;
}

int FuseOperationsInterface::power_dummy_flush(const char *a, struct fuse_file_info *b)
{
  (void) a;
  (void) b;
  return 0;
}

int FuseOperationsInterface::power_dummy_getxattr(const char *a, const char *b, char *c, size_t d)
{
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  return 0;
}
void *FuseOperationsInterface::power_init_fs (struct fuse_conn_info *conn)
{
    (void) conn;
    return 0;
}


