#include "string_helpers.h"

namespace qps {
std::string to_string(std::string str) {
  return str;
}

std::string to_string(bool b) {
  if (b) return "TRUE";
  else return "FALSE";
}

std::string to_string(Entity *entity) {
  return *entity->getEntityValue();
}
} // qps
