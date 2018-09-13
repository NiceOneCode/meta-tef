#ifndef FUSEOPERATIONS_H
#define FUSEOPERATIONS_H

#include "fuse.h"
#include <sys/stat.h>

class FuseOperationsInterface
{
public:
    FuseOperationsInterface(){}
    virtual ~FuseOperationsInterface(){}
    virtual void*power_init_fs (struct fuse_conn_info *conn) = 0;
    virtual int  power_getattr(const char *path, struct stat *stbuf) = 0;
    virtual int  power_readDir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, fuse_file_info *fi) = 0;
    virtual int  power_read(const char *path, char *buf, size_t size, off_t offset, fuse_file_info *fi) = 0;
    virtual int  power_write(const char *path, const char *buf, size_t size, off_t offset, fuse_file_info *fi) = 0;
    virtual int  power_dummy_statfs(const char *a, struct statvfs *b);
    virtual int  power_dummy_setxattr(const char *a, const char *b, const char *c, size_t d, int x);
    virtual int  power_dummy_getxattr(const char *a, const char *b, char *c, size_t d);
    virtual int  power_dummy_reloc(const char *a, off_t b);
    virtual int  power_dummy_flush(const char *a, fuse_file_info *b);
};

#endif // FUSEOPERATIONS_H
