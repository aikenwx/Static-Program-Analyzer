#include "right_paren_token.h"

#include <cassert>
#include <string>

namespace token {
auto RightParenToken::CreateToken(std::string_view str) -> std::unique_ptr<Token> {
  assert(str == ")"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<RightParenToken>(new RightParenToken());
};

auto RightParenToken::GetValue() const -> std::string { return ")"; };
}  // namespace token
