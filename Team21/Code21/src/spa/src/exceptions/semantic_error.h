#include <string>

namespace exceptions {
class SemanticError : public std::exception {
 public:
  explicit SemanticError(const std::string& message) {
    this->message = "Semantic error: " + message;
  };
  const char* what() const throw() override {
    return message.c_str();
  };
 private:
  std::string message;
};
}  // namespace exceptions
