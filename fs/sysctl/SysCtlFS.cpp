#include "SysCtlFS.h"
#include "Kern.h"
#include "Utils.h"
#include <cstring>
#include <iostream>
#include <list>
#include <sys/sysctl.h>
#include <unistd.h>

#define SYSCTL_VERBOSE 1

#if SYSCTL_VERBOSE
#define SYSCTL_LOG(__args) std::cout << "[SYSCTL] " << __args << std::endl;
#else
#define SYSCTL_LOG(__args)
#endif

/*
 * Entry fillers.
 */

using Filler = void (*)(Directory * const dir);


static void
fill_dummy(Directory * const dir)
{

}

static std::map<int, Filler> s_fillers =
{
  { CTL_KERN    , kern::fill   },
  { CTL_VM      , fill_dummy  },
  { CTL_FS      , fill_dummy  },
  { CTL_NET     , fill_dummy  },
  { CTL_DEBUG   , fill_dummy  },
  { CTL_HW      , fill_dummy  },
  { CTL_MACHDEP , fill_dummy  },
  { CTL_DDB     , fill_dummy  },
  { CTL_VFS     , fill_dummy  }
};

/*
 * Filesystem class.
 */

SysCtlFS::SysCtlFS()
  : m_startTime()
  , m_root()
{
  struct ctlname NAMES[] = CTL_NAMES;
  Directory * root = new Directory("/");
  /*
   * Get the mount time.
   */
  clock_gettime(CLOCK_REALTIME, &m_startTime);
  /*
   * Populate the root directory.
   */
  for (unsigned long i = 0; i < LEN(NAMES); i += 1) {
    switch (i) {
      case 0:
      case 8: {
        break;
      }
      default: {
        Directory * dir = new Directory(NAMES[i].ctl_name);
        s_fillers[i](dir);
        root->add(Entry::Ref(dir));
        break;
      }
    }
  }
  /*
   * Set the root entry.
   */
  m_root.reset(root);
}

int
SysCtlFS::getattr(const char * const path, struct stat * const statbuf) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  memset(statbuf, 0, sizeof(*statbuf));
  /*
   * Get the entry and set its type.
   */
  Entry const & e = getEntry(path);
  switch(e.type()) {
    case Entry::Type::File: {
      statbuf->st_mode = S_IFREG;
      break;
    }
    case Entry::Type::Directory: {
      statbuf->st_mode = S_IFDIR | S_IXUSR | S_IXGRP;
      break;
    }
  }
  /*
   * Fill-in default stats values.
   */
  statbuf->st_uid = m_uid;
  statbuf->st_gid = m_gid;
  statbuf->st_mode |= S_IRUSR | S_IRGRP | S_IROTH;
  memcpy(&statbuf->st_atim, &m_startTime, sizeof(m_startTime));
  memcpy(&statbuf->st_mtim, &m_startTime, sizeof(m_startTime));
  memcpy(&statbuf->st_ctim, &m_startTime, sizeof(m_startTime));
  return 0;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::access(const char * const path, const int mask) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * If we only check for existence, return success.
   */
  if (mask == F_OK) {
    return 0;
  }
  /*
   * If the UID or GID are wrong, abort.
   */
  if (getuid() != m_uid && getgid() != m_gid) {
    return -EPERM;
  }
  /*
   * Check the file type.
   */
  switch(e.type()) {
    case Entry::Type::File: {
      if (mask & W_OK || mask & W_OK) {
        break;
      }
      return 0;
    }
    case Entry::Type::Directory: {
      if (mask & W_OK) {
        break;
      }
      return 0;
    }
  }
  /*
   * Set the invalid argument error.
   */
  return -EACCES;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::opendir(const char * const path, struct fuse_file_info * const fi) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * Fill-in the file info.
   */
  if (e.type() != Entry::Type::Directory) {
    return -ENOTDIR;
  }
  return 0;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::readdir(const char * const path, void * const buf,
                  const fuse_fill_dir_t filler, const off_t offset,
                  struct fuse_file_info * const fi) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * Fill-in the file info.
   */
  if (e.type() != Entry::Type::Directory) {
    SYSCTL_LOG("not directory");
    return -ENOTDIR;
  }
  Directory const & dir = static_cast<Directory const &>(e);
  /*
   * Read the directory.
   */
  for (auto const & i : dir.entries()) {
    if (filler(buf, i.first.c_str(), NULL, 0) != 0) {
      return -ENOMEM;
    }
  }
  return 0;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::releasedir(const char * const path, struct fuse_file_info * const fi) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * Fill-in the file info.
   */
  if (e.type() != Entry::Type::Directory) {
    return -ENOTDIR;
  }
  return 0;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::open(const char * const path, struct fuse_file_info * const fi) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * Fill-in the file info.
   */
  if (e.type() != Entry::Type::File) {
    return -EISDIR;
  }
  return 0;
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

int
SysCtlFS::read(const char * const path, char * const buf, const size_t size,
               const off_t offset, struct fuse_file_info * const fi) try
{
  SYSCTL_LOG(__PRETTY_FUNCTION__ << ": " << path << " " << offset);
  /*
   * Get the entry.
   */
  Entry const & e = getEntry(path);
  /*
   * Check if it's a file.
   */
  if (e.type() != Entry::Type::File) {
    return -EISDIR;
  }
  File const & file = static_cast<File const &>(e);
  return file.read(buf, size, offset);
}
catch (std::invalid_argument & e)
{
  SYSCTL_LOG("No such entry: " << path);
  return -ENOENT;
}

Entry const &
SysCtlFS::getEntry(std::string const & path) const
{
  std::list<std::string> parts;
  split(path, '/', parts);
  Entry::Ref entry = m_root;
  for (auto const & name : parts) {
    if (entry->type() != Entry::Type::Directory) {
      throw std::invalid_argument(path);
    }
    Directory const & dir = static_cast<Directory const &>(*entry);
    auto e = dir.entries().find(name);
    if (e == dir.entries().end()) {
      throw std::invalid_argument(path);
    }
    entry = e->second;
  }
  return *entry;
}
