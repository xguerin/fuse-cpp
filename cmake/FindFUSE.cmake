# - Find FUSE 
# Find the native FUSE includes and library
#
#  FUSE_INCLUDE_DIRS - where to find re2.h, etc.
#  FUSE_LIBRARIES    - List of libraries when using FUSE.
#  FUSE_COMPILER     - where the FUSE compiler is.
#  FUSE_FOUND        - True if FUSE found.

find_path(FUSE_INCLUDE_DIR
  NAMES fuse.h
  NO_DEFAULT_PATH
  PATHS
  /usr/local
  /usr
  PATH_SUFFIXES include
  HINTS
  ${FUSE_ROOT}
  $ENV{FUSE_ROOT})

find_library(FUSE_LIBRARY
  NAMES fuse
  NO_DEFAULT_PATH
  PATHS
  /usr/local
  /usr
  PATH_SUFFIXES lib
  HINTS
  ${FUSE_ROOT}
  $ENV{FUSE_ROOT})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(FUSE DEFAULT_MSG FUSE_LIBRARY FUSE_INCLUDE_DIR)

if(FUSE_FOUND)
  set(FUSE_LIBRARIES ${FUSE_LIBRARY})
  set(FUSE_INCLUDE_DIRS ${FUSE_INCLUDE_DIR})
endif()
