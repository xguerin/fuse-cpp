#include "Kern.h"
#include "Utils.h"
#include <cerrno>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <sys/malloc.h>
#include <sys/namei.h>
#include <sys/sched.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/tty.h>
#include <sys/vmmeter.h>

namespace kern {

ssize_t
Malloc::Bucket::File::read(void * const buf, const size_t n, const size_t offset) const
{
  struct kmembuckets value;
  size_t len = sizeof(value);
  /*
   * Grab the bucket info.
   */
  int mib0[4] = { CTL_KERN, KERN_MALLOCSTATS, KERN_MALLOC_BUCKET, m_size };
  if (sysctl(mib0, 4, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Generate the JSON view of the structure.
   */
  std::ostringstream oss;
  oss << "{";
	oss << '"' << "calls"           << '"' << ":" << value.kb_calls      << ",";
	oss << '"' << "total_allocated" << '"' << ":" << value.kb_total      << ",";
	oss << '"' << "total_free"      << '"' << ":" << value.kb_totalfree  << ",";
	oss << '"' << "elements"        << '"' << ":" << value.kb_elmpercl   << ",";
	oss << '"' << "high_watermark"  << '"' << ":" << value.kb_highwat    << ",";
	oss << '"' << "could_free"      << '"' << ":" << value.kb_couldfree  << "}";
  oss << std::endl;
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

Malloc::Bucket::Bucket(std::string const & name, std::string const & lst)
  : Directory(name)
{
  std::list<std::string> parts;
  /*
   * Split the list and create the entries.
   */
  split(lst, ',', parts);
  for (auto const & e: parts) {
    int size = 0;
    std::istringstream(e) >> size;
    add(Entry::Ref(new File(e, size)));
  }
}

ssize_t
Malloc::KMemStats::File::read(void * const buf, const size_t n, const size_t offset) const
{
  struct kmemstats value;
  size_t len = sizeof(value);
  /*
   * Grab the bucket info.
   */
  int mib0[4] = { CTL_KERN, KERN_MALLOCSTATS, KERN_MALLOC_KMEMSTATS, m_index };
  if (sysctl(mib0, 4, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Generate the JSON view of the structure.
   */
  std::ostringstream oss;
  oss << "{";
	oss << '"' << "inuse"     << '"' << ":" << value.ks_inuse     << ",";
	oss << '"' << "calls"     << '"' << ":" << value.ks_calls     << ",";
	oss << '"' << "memuse"    << '"' << ":" << value.ks_memuse    << ",";
	oss << '"' << "limblocks" << '"' << ":" << value.ks_limblocks << ",";
	oss << '"' << "mapblocks" << '"' << ":" << value.ks_mapblocks << ",";
	oss << '"' << "maxused"   << '"' << ":" << value.ks_maxused   << ",";
	oss << '"' << "limit"     << '"' << ":" << value.ks_limit     << ",";
	oss << '"' << "size"      << '"' << ":" << value.ks_size      << ",";
	oss << '"' << "spare"     << '"' << ":" << value.ks_spare     << "}";
  oss << std::endl;
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

Malloc::KMemStats::KMemStats(std::string const & name, std::string const & lst)
  : Directory(name)
{
  /*
   * Split the list of formatted entries.
   */
  std::vector<std::string> names;
  split(lst, ',', names);
  /*
   * Create the list of IDs.
   */
  std::vector<int> indexes;
  const char * NAMES[] = INITKMEMNAMES;
  for (unsigned long i = 0; i < M_LAST; i += 1) {
    if (NAMES[i] != NULL) {
      indexes.push_back(i);
    }
  }
  /*
   * Make sure both lists have the same size.
   */
  if (names.size() != indexes.size()) {
    std::cerr << names.size() << "/" << indexes.size() << std::endl;
    throw std::runtime_error("inconsistent kmemstat items");
  }
  /*
   * Create the files.
   */
  for (size_t i = 0; i < names.size(); i += 1) {
    add(Entry::Ref(new File(names[i], indexes[i])));
  }
}

Malloc::Malloc(std::string const & name)
  : Directory(name)
{
  char buffer[4096];
  size_t len = 0;
  struct ctlname NAMES[] = CTL_KERN_MALLOC_NAMES;
  /*
   * Grab the bucket size list.
   */
  len = sizeof(buffer);
  int mib0[3] = { CTL_KERN, KERN_MALLOCSTATS, KERN_MALLOC_BUCKETS };
  if (sysctl(mib0, 3, buffer, &len, NULL, 0) == -1) {
    throw std::runtime_error("cannot get buckets");
  }
  /*
   * Create a buckets file.
   */
  add(Entry::Ref(new File(NAMES[KERN_MALLOC_BUCKETS].ctl_name)));
  /*
   * Create a bucket directory.
   */
  add(Entry::Ref(new Bucket(NAMES[KERN_MALLOC_BUCKET].ctl_name, buffer)));
  /*
   * Get the kmemnames.
   */
  len = sizeof(buffer);
  int mib1[3] = { CTL_KERN, KERN_MALLOCSTATS, KERN_MALLOC_KMEMNAMES };
  if (sysctl(mib1, 3, buffer, &len, NULL, 0) == -1) {
    throw std::runtime_error("cannot get kmemnames");
  }
  /*
   * Create a kmemnames file.
   */
  add(Entry::Ref(new File(NAMES[KERN_MALLOC_KMEMNAMES].ctl_name)));
  /*
   * Create a kmemstat directory.
   */
  add(Entry::Ref(new KMemStats(NAMES[KERN_MALLOC_KMEMSTATS].ctl_name, buffer)));
}

ssize_t
NChStats::File::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  struct nchstats value;
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_NCHSTATS };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  std::ostringstream oss;
  switch (m_index) {
    case KERN_NCHSTATS_GOODHITS : {
      oss << value.ncs_goodhits << std::endl;
      break;
    }
    case KERN_NCHSTATS_NEGHITS : {
      oss << value.ncs_neghits << std::endl;
      break;
    }
    case KERN_NCHSTATS_BADHITS : {
      oss << value.ncs_badhits << std::endl;
      break;
    }
    case KERN_NCHSTATS_FALSEHITS : {
      oss << value.ncs_falsehits << std::endl;
      break;
    }
    case KERN_NCHSTATS_MISS : {
      oss << value.ncs_miss << std::endl;
      break;
    }
    case KERN_NCHSTATS_LONG	 : {
      oss << value.ncs_long << std::endl;
      break;
    }
    case KERN_NCHSTATS_PASS2 : {
      oss << value.ncs_pass2 << std::endl;
      break;
    }
    case KERN_NCHSTATS_2PASSES : {
      std::cout << value.ncs_2passes << std::endl;
      oss << value.ncs_2passes << std::endl;
      break;
    }
    case KERN_NCHSTATS_REVHITS  : {
      oss << value.ncs_revhits << std::endl;
      break;
    }
    case KERN_NCHSTATS_REVMISS : {
      oss << value.ncs_revmiss << std::endl;
      break;
    }
    case KERN_NCHSTATS_DOTHITS : {
      oss << value.ncs_dothits << std::endl;
      break;
    }
    case KERN_NCHSTATS_DOTDOTHITS : {
      oss << value.ncs_dotdothits << std::endl;
      break;
    }
    default: {
      break;
    }
  }
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

NChStats::NChStats(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_NCHSTATS_NAMES;
  for (unsigned long i = 1; i < LEN(NAMES); i += 1) {
    add(Entry::Ref(new File(NAMES[i].ctl_name, i)));
  }
}

ssize_t
ForkStat::File::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  struct forkstat value;
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_FORKSTAT };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  std::ostringstream oss;
  switch (m_index) {
    case KERN_FORKSTAT_FORK	: {
      oss << value.cntfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_VFORK : {
      oss << value.cntvfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_TFORK : {
      oss << value.cnttfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_KTHREAD : {
      oss << value.cntkthread << std::endl;
      break;
    }
    case KERN_FORKSTAT_SIZFORK : {
      oss << value.sizfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_SIZVFORK : {
      oss << value.sizvfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_SIZTFORK : {
      oss << value.siztfork << std::endl;
      break;
    }
    case KERN_FORKSTAT_SIZKTHREAD : {
      oss << value.sizkthread << std::endl;
      break;
    }
    default: {
      break;
    }
  }
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

ForkStat::ForkStat(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_FORKSTAT_NAMES;
  for (unsigned long i = 1; i < LEN(NAMES); i += 1) {
    add(Entry::Ref(new File(NAMES[i].ctl_name, i)));
  }
}

Tty::Tty(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_TTY_NAMES;
  for (unsigned long i = 0; i < LEN(NAMES); i += 1) {
    switch (i) {
      case 0 :
      case 5 :
      case 6 :
      case 7 : {
        break;
      }
      default : {
        add(Entry::Ref(new File(NAMES[i].ctl_name, CTL_KERN, KERN_TTY, i)));
        break;
      }
    }
  }
}

SemInfo::SemInfo(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_SEMINFO_NAMES;
  for (unsigned long i = 1; i < LEN(NAMES); i += 1) {
    add(Entry::Ref(new File(NAMES[i].ctl_name, CTL_KERN, KERN_SEMINFO, i)));
  }
}

ShmInfo::ShmInfo(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_SHMINFO_NAMES;
  for (unsigned long i = 1; i < LEN(NAMES); i += 1) {
    add(Entry::Ref(new File(NAMES[i].ctl_name, CTL_KERN, KERN_SHMINFO, i)));
  }
}

TimeCounter::TimeCounter(std::string const & name)
  : Directory(name)
{
  struct ctlname NAMES[] = CTL_KERN_TIMECOUNTER_NAMES;
  for (unsigned long i = 1; i < LEN(NAMES); i += 1) {
    switch (NAMES[i].ctl_type) {
      case CTLTYPE_INT: {
        Entry * e = new Primitive<int>(NAMES[i].ctl_name, CTL_KERN, KERN_TIMECOUNTER, i);
        add(Entry::Ref(e));
        break;
      }
      case CTLTYPE_STRING: {
        Entry * e = new String(NAMES[i].ctl_name, CTL_KERN, KERN_TIMECOUNTER, i);
        add(Entry::Ref(e));
        break;
      }
      default: {
        break;
      }
    }
  }
}


ssize_t
ClockRate::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  struct clockinfo value;
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_CLOCKRATE };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  std::ostringstream oss;
  oss << "{";
	oss << '"' << "hz"      << '"' << ":" << value.hz      << ",";
	oss << '"' << "tick"    << '"' << ":" << value.tick    << ",";
	oss << '"' << "tickadj" << '"' << ":" << value.tickadj << ",";
	oss << '"' << "stathz"  << '"' << ":" << value.stathz  << ",";
	oss << '"' << "profhz"  << '"' << ":" << value.profhz  << "}";
  oss << std::endl;
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

ssize_t
BootTime::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  struct timeval value;
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_BOOTTIME };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  char buffer[1024];
  struct tm * now = localtime(&value.tv_sec);
  strftime(buffer, 1024, "%a %b %d %H:%M:%S %Y", now);
  std::ostringstream oss;
  oss << buffer << std::endl;
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

ssize_t
CpTime::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  long value[CPUSTATES];
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_CPTIME };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  std::ostringstream oss;
  oss <<"[";
  for (int i = 0; i < CPUSTATES; i += 1) {
    oss << value[i];
    if (i != CPUSTATES - 1) {
      oss << ",";
    }
  }
  oss << "]" << std::endl;
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

ssize_t
ConsDev::read(void * const buf, const size_t n, const size_t offset) const
{
  /*
   * Grab the value.
   */
  dev_t value;
  size_t len = sizeof(value);
  int mib[2] = { CTL_KERN, KERN_CONSDEV };
  if (sysctl(mib, 2, &value, &len, NULL, 0) == -1) {
    return -errno;
  }
  /*
   * Convert the value to string.
   */
  std::ostringstream oss;
  oss << devname(value, S_IFCHR) << std::endl;
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

void
fill(Directory * const dir)
{
  struct ctlname NAMES[] = CTL_KERN_NAMES;
  /*
   * Populate the root directory.
   */
  for (unsigned long i = 0; i < LEN(NAMES); i += 1) {
    switch (i) {
      case 0 :
      case 14 :
      case 15 :
      case 30 :
      case 31 :
      case 37 :
      case 48 /* MSGBUF, use dmesg */ :
      case 59 /* MBSTAT, use netstat */ :
      case 60 :
      case 65 :
      case 66 /* PROC, use ps */ :
      case 71 /* CPTIME2, disabled */ :
      case 73 /* FILE, use fstat */ : {
        break;
      }
      case KERN_CLOCKRATE : {
        Entry * e = new ClockRate(NAMES[i].ctl_name);
        dir->add(Entry::Ref(e));
        break;
      }
      case KERN_BOOTTIME : {
        Entry * e = new BootTime(NAMES[i].ctl_name);
        dir->add(Entry::Ref(e));
        break;
      }
      case KERN_CPTIME : {
        Entry * e = new CpTime(NAMES[i].ctl_name);
        dir->add(Entry::Ref(e));
        break;
      }
      case KERN_MALLOCSTATS : {
        Directory * d = new Malloc(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_NCHSTATS : {
        Directory * d = new NChStats(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_FORKSTAT : {
        Directory * d = new ForkStat(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_TTY : {
        Directory * d = new Tty(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_SEMINFO : {
        Directory * d = new SemInfo(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_SHMINFO : {
        Directory * d = new ShmInfo(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_TIMECOUNTER : {
        Directory * d = new TimeCounter(NAMES[i].ctl_name);
        dir->add(Entry::Ref(d));
        break;
      }
      case KERN_CONSDEV : {
        Entry * e = new ConsDev(NAMES[i].ctl_name);
        dir->add(Entry::Ref(e));
        break;
      }
      default : {
        Entry * e = nullptr;
        switch (NAMES[i].ctl_type) {
          case CTLTYPE_INT: {
            e = new Primitive<int>(NAMES[i].ctl_name, CTL_KERN, i);
            break;
          }
          case CTLTYPE_QUAD: {
            e = new Primitive<int64_t>(NAMES[i].ctl_name, CTL_KERN, i);
            break;
          }
          case CTLTYPE_STRING: {
            e = new String(NAMES[i].ctl_name, CTL_KERN, i);
            break;
          }
          default: {
            e = new File(NAMES[i].ctl_name);
            break;
          }
        }
        dir->add(Entry::Ref(e));
        break;
      }
    }
  }
}

}
