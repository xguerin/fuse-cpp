include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

add_library(fuse-cpp SHARED ${SOURCES})
target_compile_features(fuse-cpp PRIVATE cxx_nullptr)
target_link_libraries(fuse-cpp PRIVATE ${FUSE_LIBRARY})
