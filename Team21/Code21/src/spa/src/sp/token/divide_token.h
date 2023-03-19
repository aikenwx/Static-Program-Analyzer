#pragma once

#include <string>

#include "token.h"

namespace token {
class DivideToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::DIVIDE; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  DivideToken() = default;
};
}  // namespace token
