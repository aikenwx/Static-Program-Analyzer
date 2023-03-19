#include "semicolon_token.h"

#include <cassert>
#include <string>

namespace token {
auto SemicolonToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == ";"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<SemicolonToken>(new SemicolonToken());
};

auto SemicolonToken::GetValue() const -> std::string { return ";"; };
}  // namespace token
