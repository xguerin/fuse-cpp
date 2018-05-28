#pragma once

#include <string>

#define LEN(__names) (sizeof(__names) / sizeof(struct ctlname))

template<template <class, class> class C, class A = std::allocator<std::string>>
void
split(std::string const & s, const char d, C<std::string, A> & r) {
  r.clear();
  if (s.empty()) {
    return;
  }
  std::string buffer;
  for (auto & c : s) {
    if (c == d) {
      if (!buffer.empty()) {
        r.push_back(buffer);
      }
      buffer.clear();
    } else {
      buffer.push_back(c);
    }
  }
  if (!buffer.empty()) {
    r.push_back(buffer);
  }
}
