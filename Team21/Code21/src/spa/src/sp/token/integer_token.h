#pragma once

#include <string>

#include "token.h"

namespace token {
class IntegerToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::INTEGER; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  explicit IntegerToken(std::string_view value) : value(value) {};
  std::string value;
};
}  // namespace token
