#include <algorithm>
#include <string>

namespace util {
    auto is_identifier(std::string_view str) -> bool {
        if (str.length() == 0) {
            return false;
        }
        if (std::isdigit(str[0]) != 0) {
            return false;
        }
        return std::all_of(str.begin(), str.end(),
                           [](char chr) { return std::isalnum(chr); });
    }
}
