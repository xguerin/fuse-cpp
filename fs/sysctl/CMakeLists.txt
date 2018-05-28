add_executable(sysctlfs main.cpp SysCtlFS.cpp Kern.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-writable-strings")
target_compile_features(sysctlfs PRIVATE cxx_nullptr)
target_link_libraries(sysctlfs PRIVATE fuse-cpp)
