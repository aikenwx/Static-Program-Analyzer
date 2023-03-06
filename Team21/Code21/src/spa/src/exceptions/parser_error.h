#include <string>

namespace exceptions {
class ParserError : public std::exception {
 public:
  explicit ParserError(const std::string& message) {
    this->message = "Parser error: " + message;
  };
  const char* what() const throw() override {
    return message.c_str();
  };
 private:
  std::string message;
};
}  // namespace exceptions
