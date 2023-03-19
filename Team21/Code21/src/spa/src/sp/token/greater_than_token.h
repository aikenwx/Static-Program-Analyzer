#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterThanToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::GREATER_THAN; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  GreaterThanToken() = default;
};
}  // namespace token
