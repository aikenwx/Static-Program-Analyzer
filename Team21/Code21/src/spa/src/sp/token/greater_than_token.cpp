#include "greater_than_token.h"

#include <cassert>
#include <string>

namespace token {
auto GreaterThanToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == ">"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<GreaterThanToken>(new GreaterThanToken());
};

auto GreaterThanToken::GetValue() const -> std::string { return ">"; };
}  // namespace token
