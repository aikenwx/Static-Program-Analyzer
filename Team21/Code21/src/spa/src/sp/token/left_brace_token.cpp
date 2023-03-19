#include "left_brace_token.h"

#include <cassert>
#include <string>

namespace token {
auto LeftBraceToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "{"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LeftBraceToken>(new LeftBraceToken());
};

auto LeftBraceToken::GetValue() const -> std::string { return "{"; };
}  // namespace token
