#include "right_brace_token.h"

#include <cassert>
#include <string>

namespace token {
auto RightBraceToken::CreateToken(std::string_view str) -> std::unique_ptr<Token> {
  assert(str == "}"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<RightBraceToken>(new RightBraceToken());
};

auto RightBraceToken::GetValue() const -> std::string { return "}"; };
}  // namespace token
