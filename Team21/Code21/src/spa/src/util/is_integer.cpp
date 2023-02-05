#include <string>

namespace util {
bool is_integer(const std::string& str) {
  if (str.length() == 0) return false;
  for (const char& c : str) {
    if (std::isdigit(c) == 0) return false;
  }
  return true;
}
}  // namespace util
