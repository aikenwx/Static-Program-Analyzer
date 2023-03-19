#pragma once

#include <string>

#include "token.h"

namespace token {
class PlusToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::PLUS; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  PlusToken() = default;
};
}  // namespace token
