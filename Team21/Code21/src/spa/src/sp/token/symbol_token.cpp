#include "symbol_token.h"

namespace token {
auto SymbolToken::CreateToken(std::string_view value)
    -> std::unique_ptr<Token> {
  return std::unique_ptr<SymbolToken>(new SymbolToken(value));
};
[[nodiscard]] auto SymbolToken::GetValue() const -> std::string {
  return value_;
};
} // namespace token
