#pragma once

#include <string>

#include "token.h"

namespace token {
class AssignToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::ASSIGN; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  AssignToken() = default;
};
}  // namespace token
