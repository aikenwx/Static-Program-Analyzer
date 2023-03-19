#pragma once

#include <string>

#include "token.h"

namespace token {
class LeftBraceToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::LEFT_BRACE; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  LeftBraceToken() = default;
};
}  // namespace token
