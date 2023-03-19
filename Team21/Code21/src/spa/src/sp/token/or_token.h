#pragma once

#include <string>

#include "token.h"

namespace token {
class OrToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::OR; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  OrToken() = default;
};
}  // namespace token
