#pragma once
#include <memory>
#include <string>

namespace token {
class Token {
 public:
  static const std::unique_ptr<Token> CreateToken(std::string);
  virtual ~Token(){};
  virtual const std::string GetValue() = 0;
};
}  // namespace token
