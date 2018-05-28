#pragma once

#include <fuse.h>

namespace FUSE {

class Context {
 public:

  Context();

  int run(const int argc, char ** const argv);

 protected:

  virtual int getattr(const char * const path, struct stat * const statbuf);
  virtual int readlink(const char * const path, char *link, const size_t size);
  virtual int mknod(const char * const path, const mode_t mode, const dev_t dev);
  virtual int mkdir(const char * const path, const mode_t mode);
  virtual int unlink(const char * const path);
  virtual int rmdir(const char * const path);
  virtual int symlink(const char * const path, const char * const link);
  virtual int rename(const char * const path, const char * const newpath);
  virtual int link(const char * const path, const char * const newpath);
  virtual int chmod(const char * const path, const mode_t mode);
  virtual int chown(const char * const path, const uid_t uid, const gid_t gid);
  virtual int truncate(const char * const path, const off_t newsize);
  virtual int utime(const char * const path, struct utimbuf * const ubuf);
  virtual int open(const char * const path, struct fuse_file_info * const fi);
  virtual int read(const char * const path, char * const buf, const size_t size,
                   const off_t offset, struct fuse_file_info * const fi);
  virtual int write(const char * const path, const char * const buf,
                    const size_t size, const off_t offset,
                    struct fuse_file_info * const fi);
  virtual int statfs(const char * const path, struct statvfs * const statv);
  virtual int flush(const char * const path, struct fuse_file_info * const fi);
  virtual int release(const char * const path,
                      struct fuse_file_info * const fi);
  virtual int fsync(const char * const path, const int datasync,
                    struct fuse_file_info * const fi);

#ifdef HAVE_SYXATTR_H
  virtual int setxattr(const char * const path, const char * const name,
                       const char * const value, const size_t size,
                       const int flags);
  virtual int getxattr(const char * const path, const char * const name,
                       char * const value, const size_t size);
  virtual int listxattr(const char * const path, char * const list,
                        const size_t size);
  virtual int removexattr(const char * const path, const char * const name);
#endif

  virtual int opendir(const char * const path,
                      struct fuse_file_info * const fi);
  virtual int readdir(const char * const path, void * const buf,
                      const fuse_fill_dir_t filler, const off_t offset,
                      struct fuse_file_info * const fi);
  virtual int releasedir(const char * const path,
                         struct fuse_file_info * const fi);
  virtual int fsyncdir(const char * const path, const int datasync,
                       struct fuse_file_info * const fi);
  virtual void * init(struct fuse_conn_info * const conn);
  virtual void destroy(void * const userdata);
  virtual int access(const char * const path, const int mask);
  virtual int ftruncate(const char * const path, const off_t offset,
                        struct fuse_file_info * const fi);
  virtual int fgetattr(const char * const path, struct stat * const statbuf,
                       struct fuse_file_info * const fi);

  const uid_t m_uid;
  const uid_t m_gid;

 private:

  static int s_getattr(const char * const path, struct stat * const statbuf);
  static int s_readlink(const char * const path, char *link, const size_t size);
  static int s_mknod(const char * const path, const mode_t mode, const dev_t dev);
  static int s_mkdir(const char * const path, const mode_t mode);
  static int s_unlink(const char * const path);
  static int s_rmdir(const char * const path);
  static int s_symlink(const char * const path, const char * const link);
  static int s_rename(const char * const path, const char * const newpath);
  static int s_link(const char * const path, const char * const newpath);
  static int s_chmod(const char * const path, const mode_t mode);
  static int s_chown(const char * const path, const uid_t uid, const gid_t gid);
  static int s_truncate(const char * const path, const off_t newsize);
  static int s_utime(const char * const path, struct utimbuf * const ubuf);
  static int s_open(const char * const path, struct fuse_file_info * const fi);
  static int s_read(const char * const path, char * const buf, const size_t size,
                    const off_t offset, struct fuse_file_info * const fi);
  static int s_write(const char * const path, const char * const buf,
                     const size_t size, const off_t offset,
                     struct fuse_file_info * const fi);
  static int s_statfs(const char * const path, struct statvfs * const statv);
  static int s_flush(const char * const path, struct fuse_file_info * const fi);
  static int s_release(const char * const path,
                       struct fuse_file_info * const fi);
  static int s_fsync(const char * const path, const int datasync,
                     struct fuse_file_info * const fi);

#ifdef HAVE_SYS_XATTR_H
  static int s_setxattr(const char * const path, const char * const name,
                        const char * const value, const size_t size,
                        const int flags);
  static int s_getxattr(const char * const path, const char * const name,
                        char * const value, const size_t size);
  static int s_listxattr(const char * const path, char * const list,
                         const size_t size);
  static int s_removexattr(const char * const path, const char * const name);
#endif

  static int s_opendir(const char * const path,
                       struct fuse_file_info * const fi);
  static int s_readdir(const char * const path, void * const buf,
                       const fuse_fill_dir_t filler, const off_t offset,
                       struct fuse_file_info * const fi);
  static int s_releasedir(const char * const path,
                          struct fuse_file_info * const fi);
  static int s_fsyncdir(const char * const path, const int datasync,
                        struct fuse_file_info * const fi);
  static void * s_init(struct fuse_conn_info * const conn);
  static void s_destroy(void * const userdata);
  static int s_access(const char * const path, const int mask);
  static int s_ftruncate(const char * const path, const off_t offset,
                         struct fuse_file_info * const fi);
  static int s_fgetattr(const char * const path, struct stat * const statbuf,
                          struct fuse_file_info * const fi);

  static fuse_operations s_operations;
};

}
