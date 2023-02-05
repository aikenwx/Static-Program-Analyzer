#pragma once
#include <string>

namespace token {
class Token {
 public:
  virtual const std::string getValue() = 0;
  static const Token* createToken(std::string str);
};

class SymbolToken : public Token {
 public:
  void operator=(const SymbolToken&) = delete;
 private:
  static SymbolToken* instance_;
};
}  // namespace token