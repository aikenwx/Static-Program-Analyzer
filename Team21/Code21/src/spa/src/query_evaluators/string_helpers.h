#pragma once

#include <string>
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace qps {
std::string to_string(std::string str);

std::string to_string(bool b);

std::string to_string(Entity *entity);

template<typename T>
std::string Join(const std::vector<T> &vec, const std::string &sep) {
  std::string res;
  for (int i = 0; i < vec.size(); ++i) {
    if (i != 0) {
      res += sep;
    }
    res += to_string(vec[i]);
  }
  return res;
}
} // qps
