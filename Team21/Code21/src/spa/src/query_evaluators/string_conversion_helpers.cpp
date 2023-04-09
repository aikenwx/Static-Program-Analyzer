#include "string_conversion_helpers.h"

namespace qps {
auto to_string(std::string str) -> std::string {
  return str;
}

auto to_string(bool boolean) -> std::string {
  if (boolean) {
    return "TRUE";
  }
  return "FALSE";
}

auto to_string(Entity *entity) -> std::string {
  return *entity->getEntityValue();
}
}  // namespace qps
