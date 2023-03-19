#include "minus_token.h"

#include <cassert>
#include <string>

namespace token {
auto MinusToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "-"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<MinusToken>(new MinusToken());
};

auto MinusToken::GetValue() const -> std::string { return "-"; };
}  // namespace token
