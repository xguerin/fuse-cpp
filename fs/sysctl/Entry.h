#pragma once

#include <map>
#include <memory>
#include <string>
#include <unistd.h>

class Entry
{
 public:

  using Ref = std::shared_ptr<Entry>;

  enum class Type
  {
    File,
    Directory
  };

  Entry(const Type type, std::string const & name)
    : m_type(type)
    , m_name(name)
  {

  }

  virtual ~Entry() { }

  Type type() const
  {
    return m_type;
  }

  std::string const & name() const
  {
    return m_name;
  }

 private:

  const Type        m_type;
  const std::string m_name;
};

class File : public Entry
{
 public:

  File(std::string const & name)
    : Entry(Type::File, name)
  {

  }

  virtual ~File() { }

  virtual ssize_t read(void * const buf, const size_t n, const size_t off) const
  {
    return -1;
  }
};

class Directory : public Entry
{
 public:

  using Entries = std::map<std::string, Entry::Ref>;

  Directory(std::string const & name)
    : Entry(Type::Directory, name)
  {

  }

  void add(Entry::Ref const & entry)
  {
    m_entries.emplace(std::make_pair(entry->name(), entry));
  }

  Entries const & entries() const
  {
    return m_entries;
  }

 private:

  Entries m_entries;
};
