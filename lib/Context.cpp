#include <fuse-cpp/Context.h>
#include <iostream>
#include <unistd.h>

#define FUSE_VERBOSE 1

#if FUSE_VERBOSE
#define FUSE_LOG(__args) std::cout << "[FUSE] " << __args << std::endl;
#else
#define FUSE_LOG(__args)
#endif

namespace FUSE {

/*
 * FUSE operations structures.
 */

struct fuse_operations Context::s_operations =
{
  .getattr = Context::s_getattr,
  .readlink = Context::s_readlink,
  // no .getdir -- that's deprecated
  .getdir = NULL,
  .mknod = Context::s_mknod,
  .mkdir = Context::s_mkdir,
  .unlink = Context::s_unlink,
  .rmdir = Context::s_rmdir,
  .symlink = Context::s_symlink,
  .rename = Context::s_rename,
  .link = Context::s_link,
  .chmod = Context::s_chmod,
  .chown = Context::s_chown,
  .truncate = Context::s_truncate,
  .utime = Context::s_utime,
  .open = Context::s_open,
  .read = Context::s_read,
  .write = Context::s_write,
  /** Just a placeholder, don't set */ // huh???
  .statfs = Context::s_statfs,
  .flush = Context::s_flush,
  .release = Context::s_release,
  .fsync = Context::s_fsync,

#ifdef HAVE_SYS_XATTR_H
  .setxattr = Context::s_setxattr,
  .getxattr = Context::s_getxattr,
  .listxattr = Context::s_listxattr,
  .removexattr = Context::s_removexattr,
#endif

  .opendir = Context::s_opendir,
  .readdir = Context::s_readdir,
  .releasedir = Context::s_releasedir,
  .fsyncdir = Context::s_fsyncdir,
  .init = Context::s_init,
  .destroy = Context::s_destroy,
  .access = Context::s_access,
  .ftruncate = Context::s_ftruncate,
  .fgetattr = Context::s_fgetattr
};

/*
 * Default constructor and runner.
 */

Context::Context()
  : m_uid(getuid())
  , m_gid(getgid())
{

}

int
Context::run(const int argc, char ** const argv)
{
  return fuse_main(argc, argv, &s_operations, this);
}

/*
 * Virtual definitions.
 */

int
Context::getattr(const char * const path, struct stat * const statbuf)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::readlink(const char * const path, char *link, const size_t size)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::mknod(const char * const path, const mode_t mode, const dev_t dev)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::mkdir(const char * const path, const mode_t mode)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::unlink(const char * const path)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::rmdir(const char * const path)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::symlink(const char * const path, const char * const link)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::rename(const char * const path, const char * const newpath)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::link(const char * const path, const char * const newpath)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::chmod(const char * const path, const mode_t mode)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::chown(const char * const path, const uid_t uid, const gid_t gid)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::truncate(const char * const path, const off_t newsize)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::utime(const char * const path, struct utimbuf * const ubuf)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::open(const char * const path, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::read(const char * const path, char * const buf, const size_t size,
              const off_t offset, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::write(const char * const path, const char * const buf, const size_t size,
               const off_t offset, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::statfs(const char * const path, struct statvfs * const statv)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::flush(const char * const path, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::release(const char * const path, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::fsync(const char * const path, int datasync,
               struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}


#ifdef HAVE_SYS_XATTR_H
int
Context::setxattr(const char * const path, const char * const name,
                  const char * const value, const size_t size, const int flags)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::getxattr(const char * const path, const char * const name,
                  char * const value, const size_t size)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::listxattr(const char * const path, char * const list, const size_t size)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::removexattr(const char * const path, const char * const name)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

#endif

int
Context::opendir(const char * const path, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::readdir(const char * const path, void * const buf,
                 const fuse_fill_dir_t filler, const off_t offset,
                 struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::releasedir(const char * const path, struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::fsyncdir(const char * const path, const int datasync,
                  struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

void *
Context::init(struct fuse_conn_info * const conn)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return nullptr;
}

void Context::destroy(void * const userdata)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
}

int
Context::access(const char * const path, int mask)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::ftruncate(const char * const path, off_t offset,
                   struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

int
Context::fgetattr(const char * const path, struct stat * const statbuf,
                  struct fuse_file_info * const fi)
{
  FUSE_LOG(__PRETTY_FUNCTION__);
  return -1;
}

/*
 * Static trampolines.
 */

int
Context::s_getattr(const char * const path, struct stat * const statbuf)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->getattr(path, statbuf);
}

int
Context::s_readlink(const char * const path, char *link, const size_t size)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->readlink(path, link, size);
}

int
Context::s_mknod(const char * const path, const mode_t mode, const dev_t dev)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->mknod(path, mode, dev);
}

int
Context::s_mkdir(const char * const path, const mode_t mode)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->mkdir(path, mode);
}

int
Context::s_unlink(const char * const path)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->unlink(path);
}

int
Context::s_rmdir(const char * const path)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->rmdir(path);
}

int
Context::s_symlink(const char * const path, const char * const link)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->symlink(path, link);
}

int
Context::s_rename(const char * const path, const char * const newpath)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->rename(path, newpath);
}

int
Context::s_link(const char * const path, const char * const newpath)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->link(path, newpath);
}

int
Context::s_chmod(const char * const path, const mode_t mode)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->chmod(path, mode);
}

int
Context::s_chown(const char * const path, const uid_t uid, const gid_t gid)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->chown(path, uid, gid);
}

int
Context::s_truncate(const char * const path, const off_t newsize)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->truncate(path, newsize);
}

int
Context::s_utime(const char * const path, struct utimbuf * const ubuf)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->utime(path, ubuf);
}

int
Context::s_open(const char * const path, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->open(path, fi);
}

int
Context::s_read(const char * const path, char * const buf, const size_t size,
                const off_t offset, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->read(path, buf, size, offset, fi);
}

int
Context::s_write(const char * const path, const char * const buf,
                 const size_t size, const off_t offset,
                 struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->write(path, buf, size, offset, fi);
}

int
Context::s_statfs(const char * const path, struct statvfs * const statv)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->statfs(path, statv);
}

int
Context::s_flush(const char * const path, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->flush(path, fi);
}

int
Context::s_release(const char * const path, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->release(path, fi);
}

int
Context::s_fsync(const char * const path, const int datasync,
                 struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->fsync(path, datasync, fi);
}

#ifdef HAVE_SYS_XATTR_H
int
Context::s_setxattr(const char * const path, const char * const name,
                    const char * const value, const size_t size, const int flags)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return setxattr(path, name, value, size, flags);
}

int
Context::s_getxattr(const char * const path, const char * const name,
                    char * const value, const size_t size)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return getxattr(path, name, value, size);
}

int
Context::s_listxattr(const char * const path, char * const list, const size_t size)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->listxattr(path, list, size);
}

int
Context::s_removexattr(const char * const path, const char * const name)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return removexattr(path, name);
}

#endif

int
Context::s_opendir(const char * const path, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->opendir(path, fi);
}

int
Context::s_readdir(const char * const path, void * const buf,
                   const fuse_fill_dir_t filler, const off_t offset,
                   struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->readdir(path, buf, filler, offset, fi);
}

int
Context::s_releasedir(const char * const path, struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->releasedir(path, fi);
}

int
Context::s_fsyncdir(const char * const path, const int datasync,
                    struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->fsyncdir(path, datasync, fi);
}

void *
Context::s_init(struct fuse_conn_info * const conn)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->init(conn);;
}

void Context::s_destroy(void * const userdata)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  c->destroy(userdata);
}

int
Context::s_access(const char * const path, const int mask)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->access(path, mask);
}

int
Context::s_ftruncate(const char * const path, const off_t offset,
                     struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->ftruncate(path, offset, fi);
}

int
Context::s_fgetattr(const char * const path, struct stat * const statbuf,
                    struct fuse_file_info * const fi)
{
  Context * c = reinterpret_cast<Context *>(fuse_get_context()->private_data);
  return c->fgetattr(path, statbuf, fi);
}

}
