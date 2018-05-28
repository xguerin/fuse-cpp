#pragma once

#include "Entry.h"
#include <fuse-cpp/Context.h>
#include <time.h>

class SysCtlFS : public FUSE::Context {
 public:

  SysCtlFS();

 protected:

  int getattr(const char * const path, struct stat * const statbuf);
  int access(const char * const path, const int mask);
  int opendir(const char * const path, struct fuse_file_info * const fi);
  int readdir(const char * const path, void * const buf,
              const fuse_fill_dir_t filler, const off_t offset,
              struct fuse_file_info * const fi);
  int releasedir(const char * const path, struct fuse_file_info * const fi);
  int open(const char * const path, struct fuse_file_info * const fi);
  int read(const char * const path, char * const buf, const size_t size,
           const off_t offset, struct fuse_file_info * const fi);

 private:

  Entry const & getEntry(std::string const & path) const;

  struct timespec m_startTime;
  Entry::Ref      m_root;
};
