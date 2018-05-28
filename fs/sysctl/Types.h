#pragma once

#include "Entry.h"
#include <cstring>
#include <sstream>
#include <sys/sysctl.h>

template<class T>
class Primitive : public File
{
 public:

  Primitive(std::string const & name, const int a, const int b)
    : File(name)
    , m_a(a)
    , m_b(b)
    , m_c(0)
    , m_n(2)
  {

  }

  Primitive(std::string const & name, const int a, const int b, const int c)
    : File(name)
    , m_a(a)
    , m_b(b)
    , m_c(c)
    , m_n(3)
  {

  }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const
  {
    T value = 0;
    size_t len = sizeof(T);
    int mib[m_n];
    /*
     * Grab the value.
     */
    mib[0] = m_a;
    mib[1] = m_b;
    if (m_n == 3) {
      mib[2] = m_c;
    }
    if (sysctl(mib, m_n, &value, &len, NULL, 0) == -1) {
      return -errno;
    }
    /*
     * Convert the value to string.
     */
    std::ostringstream oss;
    oss << value << std::endl;
    size_t slen = oss.str().length() + 1;
    /*
     * Check that there is enough space to read.
     */
    if (n < slen) {
      return -ENOMEM;
    }
    /*
     * Check if there is any offset.
     */
    if (offset > 0) {
      return 0;
    }
    /*
     * Copy the value.
     */
    memcpy(buf, oss.str().c_str(), slen);
    return slen;
  }

 private:

  const int m_a;
  const int m_b;
  const int m_c;
  const int m_n;
};

class String : public File
{
 public:

  String(std::string const & name, const int a, const int b)
    : File(name)
    , m_a(a)
    , m_b(b)
    , m_c(0)
    , m_n(2)
  {

  }

  String(std::string const & name, const int a, const int b, const int c)
    : File(name)
    , m_a(a)
    , m_b(b)
    , m_c(c)
    , m_n(3)
  {

  }

  ssize_t read(void * const buf, const size_t n, const size_t offset) const
  {
    char value[4096];
    size_t len = sizeof(value);
    int mib[m_n];
    /*
     * Grab the value.
     */
    mib[0] = m_a;
    mib[1] = m_b;
    if (m_n == 3) {
      mib[2] = m_c;
    }
    if (sysctl(mib, m_n, value, &len, NULL, 0) == -1) {
      return -errno;
    }
    /*
     * Convert the value to string.
     */
    std::ostringstream oss;
    oss << value << std::endl;
    size_t slen = oss.str().length() + 1;
    /*
     * Check that there is enough space to read.
     */
    if (n < slen) {
      return -ENOMEM;
    }
    /*
     * Check if there is any offset.
     */
    if (offset > 0) {
      return 0;
    }
    /*
     * Copy the value.
     */
    memcpy(buf, oss.str().c_str(), slen);
    return slen;
  }

 private:

  const int m_a;
  const int m_b;
  const int m_c;
  const int m_n;
};
