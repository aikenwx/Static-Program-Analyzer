#pragma once

#include <string>

#include "token.h"

namespace token {
class MinusToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::MINUS; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  MinusToken() = default;
};
}  // namespace token
