#include "../symbol_token.h"
#include "../token.h"
#include "util/instance_of.h"
#include <algorithm>
#include <initializer_list>
#include <memory>

namespace token {
[[nodiscard]] auto IsTokenSymbolValue(const Token *token,
                                      std::string_view value) -> bool;

[[nodiscard]] auto
IsTokenSymbolValue(const std::shared_ptr<token::Token> &token_ref,
                   std::string_view value) -> bool;

// separate method used to indicate that a unique pointer can be safely passed
// into this function without any issues, e.g. with the pointer being
// invalidated or more references to this pointer being created
[[nodiscard]] auto
IsTokenSymbolValue(const std::unique_ptr<token::Token> &token_ref,
                   std::string_view value) -> bool;
[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const Token *token,
                        std::initializer_list<std::string_view> values) -> bool;

[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const std::shared_ptr<token::Token> &token_ref,
                        std::initializer_list<std::string_view> values) -> bool;

// separate method used to indicate that a unique pointer can be safely passed
// into this function without any issues, e.g. with the pointer being
// invalidated or more references to this pointer being created
[[nodiscard]] auto
IsTokenSymbolValueAnyOf(const std::unique_ptr<token::Token> &token_ref,
                        std::initializer_list<std::string_view> values) -> bool;
} // namespace token
