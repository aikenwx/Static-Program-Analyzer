#pragma once

#include <string>

#include "token.h"

namespace token {
class MultiplyToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::MULTIPLY; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  MultiplyToken() = default;
};
}  // namespace token
