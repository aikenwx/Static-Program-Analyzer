#include "is_symbol_value.h"
#include "../symbol_token.h"
#include "util/instance_of.h"
#include <algorithm>

namespace token {
[[nodiscard]] auto IsTokenSymbolValue(const Token *token,
                                      std::string_view value) -> bool {
  return util::instance_of<token::SymbolToken>(token) &&
         token->GetValue() == value;
}

[[nodiscard]] auto
IsTokenSymbolValue(const std::shared_ptr<token::Token> &token_ref,
                   std::string_view value) -> bool {
  return IsTokenSymbolValue(token_ref.get(), value);
}

// separate method used to indicate that a unique pointer can be safely passed
// into this function without any issues, e.g. with the pointer being
// invalidated or more references to this pointer being created
[[nodiscard]] auto
IsTokenSymbolValue(const std::unique_ptr<token::Token> &token_ref,
                   std::string_view value) -> bool {
  return IsTokenSymbolValue(token_ref.get(), value);
}

[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const Token *token,
                        std::initializer_list<std::string_view> values)
    -> bool {
  if (!util::instance_of<token::SymbolToken>(token)) {
    return false;
  };

  return std::any_of(values.begin(), values.end(), [token](auto value) {
    return token->GetValue() == value;
  });
}

[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const std::shared_ptr<token::Token> &token_ref,
                        std::initializer_list<std::string_view> values)
    -> bool {
  return IsTokenSymbolValueAnyOf(token_ref.get(), values);
}

// separate method used to indicate that a unique pointer can be safely passed
// into this function without any issues, e.g. with the pointer being
// invalidated or more references to this pointer being created
[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const std::unique_ptr<token::Token> &token_ref,
                        std::initializer_list<std::string_view> values)
    -> bool {
  return IsTokenSymbolValueAnyOf(token_ref.get(), values);
}
} // namespace token
