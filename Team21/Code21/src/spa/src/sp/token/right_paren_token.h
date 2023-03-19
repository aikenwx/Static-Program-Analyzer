#pragma once

#include <string>

#include "token.h"

namespace token {
class RightParenToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::RIGHT_PAREN; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  RightParenToken() = default;
};
}  // namespace token
