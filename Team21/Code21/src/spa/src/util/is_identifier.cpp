#include <string>

namespace util {
    bool is_identifier(const std::string& str) {
        if (str.length() == 0) {
            return false;
        }
        if (std::isdigit(str[0]) != 0) {
            return false;
        }
        for (const char& c : str) {
            if (std::isalnum(c) == 0) {
                return false;
            }
        }
        return true;
    }
}
