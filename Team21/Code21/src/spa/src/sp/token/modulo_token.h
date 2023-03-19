#pragma once

#include <string>

#include "token.h"

namespace token {
class ModuloToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetType() const -> TokenType override { return TokenType::MODULO; }
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  ModuloToken() = default;
};
}  // namespace token
