#pragma once

#include <string>

#include "token.h"

namespace token {
class GreaterEqualToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override {
    return TokenType::GREATER_EQUAL;
  }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  GreaterEqualToken() = default;
};
}  // namespace token
