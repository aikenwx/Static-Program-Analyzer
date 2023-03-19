#pragma once

#include <string>

#include "token.h"

namespace token {
class NotToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::NOT; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  NotToken() = default;
};
}  // namespace token
