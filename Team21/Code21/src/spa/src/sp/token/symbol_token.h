#pragma once

#include <string>

#include "token.h"

namespace token {
class SymbolToken : public Token {
public:
  static auto CreateToken(std::string_view value) -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetValue() const -> std::string override;

private:
  explicit SymbolToken(std::string_view value) : value_(value){};
  std::string value_;
};
} // namespace token
