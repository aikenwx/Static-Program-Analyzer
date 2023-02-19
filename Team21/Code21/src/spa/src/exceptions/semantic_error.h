#include <string>

namespace exceptions {
class SemanticError : public std::exception {
 public:
  SemanticError(std::string message) {
    this->message = "Semantic error: " + message;
  };
  const char* what() const throw() {
    return message.c_str();
  };
 private:
  std::string message;
};
}  // namespace exceptions
