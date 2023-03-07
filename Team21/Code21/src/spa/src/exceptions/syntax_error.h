#include <string>

namespace exceptions {
class SyntaxError : public std::exception {
 public:
  explicit SyntaxError(const std::string& message) {
    this->message = "Syntax error: " + message;
  };
  const char* what() const throw() override {
    return message.c_str();
  };
 private:
  std::string message;
};
}  // namespace exceptions
