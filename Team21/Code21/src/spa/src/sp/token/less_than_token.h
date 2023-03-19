#pragma once

#include <string>

#include "token.h"

namespace token {
class LessThanToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::LESS_THAN; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  LessThanToken() = default;
};
}  // namespace token
