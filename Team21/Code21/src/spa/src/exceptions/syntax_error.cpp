#include "syntax_error.h"

#include <string>

namespace exceptions {
SyntaxError::SyntaxError(std::string message) { this->message = message; }
const char* SyntaxError::what() const throw() { return "Syntax error"; }
}  // namespace exceptions
