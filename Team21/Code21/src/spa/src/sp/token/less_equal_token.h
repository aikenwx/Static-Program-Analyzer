#pragma once

#include <string>

#include "token.h"

namespace token {
class LessEqualToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::LESS_EQUAL; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  LessEqualToken() = default;
};
}  // namespace token
