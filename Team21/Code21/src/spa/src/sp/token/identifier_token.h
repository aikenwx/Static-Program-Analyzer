#pragma once

#include <string>

#include "token.h"

namespace token {
class IdentifierToken : public Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetValue() const -> std::string override;

 private:
  explicit IdentifierToken(std::string_view value) : value(value) {};
  std::string value;
};
}  // namespace token
