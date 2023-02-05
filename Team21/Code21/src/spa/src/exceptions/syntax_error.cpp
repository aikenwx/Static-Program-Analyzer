#include "syntax_error.h"

#include <string>

namespace exceptions {
SyntaxError::SyntaxError(std::string message) { this->message = message; }
}  // namespace exceptions