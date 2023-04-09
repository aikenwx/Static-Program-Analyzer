#pragma once

#include <string>
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace qps {
auto to_string(std::string str) -> std::string;

auto to_string(bool boolean) -> std::string;

auto to_string(Entity *entity) -> std::string;

template<typename T>
auto Join(const std::vector<T> &vec, const std::string &sep) -> std::string {
  std::string res;
  for (int i = 0; i < vec.size(); ++i) {
    if (i != 0) {
      res += sep;
    }
    res += to_string(vec[i]);
  }
  return res;
}
}  // namespace qps
