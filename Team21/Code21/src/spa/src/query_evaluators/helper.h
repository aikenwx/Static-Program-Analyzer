#pragma once

#include <string>
#include "PKBStorageClasses/EntityClasses/Entity.h"

namespace qps {

std::string to_string(std::string str) {
  return str;
}

std::string to_string(Entity *entity) {
  return *entity->getEntityValue();
}

template<typename T>
std::string Join(std::vector<T> vec, const std::string &sep) {
  std::string res;
  for (int i = 0; i < vec.size(); ++i) {
    if (i != 0) res += sep;
    res += to_string(vec[i]);
  }
  return res;
}
}
