#include <string>

namespace exceptions {
class SyntaxError : public std::exception {
 public:
  SyntaxError(std::string message);
  const char* what() const throw();
 private:
  std::string message;
};
}  // namespace exceptions