#include "left_paren_token.h"

#include <cassert>
#include <string>

namespace token {
auto LeftParenToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "("); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LeftParenToken>(new LeftParenToken());
};

auto LeftParenToken::GetValue() const -> std::string { return "("; };
}  // namespace token
