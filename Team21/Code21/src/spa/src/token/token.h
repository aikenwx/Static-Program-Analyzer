#pragma once
#include <memory>
#include <string>

namespace token {
class Token {
 public:
  static const std::unique_ptr<Token> createToken(std::string);
  virtual ~Token(){};
  virtual const std::string getValue() = 0;
};
}  // namespace token
