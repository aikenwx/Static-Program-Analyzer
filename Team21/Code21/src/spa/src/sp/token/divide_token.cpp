#include "divide_token.h"

#include <cassert>
#include <string>

namespace token {
auto DivideToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "/"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<DivideToken>(new DivideToken());
};

auto DivideToken::GetValue() const -> std::string { return "/"; };
}  // namespace token
