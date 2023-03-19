#include "greater_equal_token.h"

#include <cassert>
#include <string>

namespace token {
auto GreaterEqualToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == ">="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<GreaterEqualToken>(new GreaterEqualToken());
};

auto GreaterEqualToken::GetValue() const -> std::string { return ">="; };
}  // namespace token
