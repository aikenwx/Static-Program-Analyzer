#include <algorithm>
#include <string>
#include <string_view>

namespace util {
auto is_integer(std::string_view str) -> bool {
  if (str.length() == 0) {
    return false;
  }
  return std::all_of(str.begin(), str.end(),
                     [](char chr) { return std::isdigit(chr); });
}
} // namespace util
