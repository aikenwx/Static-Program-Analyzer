#include "equal_token.h"

#include <cassert>
#include <string>

namespace token {
auto EqualToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "=="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<EqualToken>(new EqualToken());
};

auto EqualToken::GetValue() const -> std::string { return "=="; };
}  // namespace token
