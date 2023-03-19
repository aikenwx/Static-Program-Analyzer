#pragma once

#include <string>

#include "token.h"

namespace token {
class SemicolonToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::SEMICOLON; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  SemicolonToken() = default;
};
}  // namespace token
