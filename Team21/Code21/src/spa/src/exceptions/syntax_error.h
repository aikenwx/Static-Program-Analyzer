#include <string>

namespace exceptions {
class SyntaxError : public std::exception {
 public:
  SyntaxError(std::string message) {
    this->message = "Syntax error: " + message;
  };
  const char* what() const throw() {
    return message.c_str();
  };
 private:
  std::string message;
};
}  // namespace exceptions
