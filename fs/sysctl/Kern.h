#pragma once

#include "Entry.h"
#include "Types.h"

namespace kern {

class Malloc : public Directory
{
 public:

  Malloc(std::string const & name);

 private:

  class Bucket : public Directory
  {
   public:

    Bucket(std::string const & name, std::string const & lst);

   private:

    struct File : public ::File
    {
     public:

      File(std::string const & name, const int size)
        : ::File(name)
        , m_size(size)
      { }

      ssize_t read(void * const buf, const size_t n, const size_t offset) const;

     private:

      const int m_size;
    };
  };

  class KMemStats : public Directory
  {
   public:

    KMemStats(std::string const & name, std::string const & lst);

   private:

    struct File : public ::File
    {
     public:

      File(std::string const & name, const int index)
        : ::File(name)
        , m_index(index)
      { }

      ssize_t read(void * const buf, const size_t n, const size_t offset) const;

     private:

      const int m_index;
    };
  };
};

class NChStats : public Directory
{
 public:

  NChStats(std::string const & name);

  class File : public ::File
  {
   public:

    File(std::string const & name, const int index)
      : ::File(name)
      , m_index(index) { }

    ssize_t read(void * const buf, const size_t n, const size_t offset) const;

   private:

    const int m_index;
  };
};

class ForkStat : public Directory
{
 public:

  ForkStat(std::string const & name);

 private:

  class File : public ::File
  {
   public:

    File(std::string const & name, const int index)
      : ::File(name)
      , m_index(index) { }

    ssize_t read(void * const buf, const size_t n, const size_t offset) const;

   private:

    const int m_index;
  };
};

class Tty : public Directory
{
 public:

  Tty(std::string const & name);

 private:

  using File = Primitive<int64_t>;
};

class SemInfo : public Directory
{
 public:

  SemInfo(std::string const & name);

 private:

  using File = Primitive<int>;
};

class ShmInfo : public Directory
{
 public:

  ShmInfo(std::string const & name);

 private:

  using File = Primitive<int>;
};

class TimeCounter : public Directory
{
 public:

  TimeCounter(std::string const & name);
};

class ClockRate : public File
{
 public:

  ClockRate(std::string const & name)
    : File(name) { }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const;
};

class BootTime : public File
{
 public:

  BootTime(std::string const & name)
    : File(name) { }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const;
};

class CpTime : public File
{
 public:

  CpTime(std::string const & name)
    : File(name) { }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const;
};

class ConsDev : public File
{
 public:

  ConsDev(std::string const & name)
    : File(name) { }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const;
};


void fill(Directory * const dir);

}
