#include "less_equal_token.h"

#include <cassert>
#include <string>

namespace token {
auto LessEqualToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "<="); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<LessEqualToken>(new LessEqualToken());
};

auto LessEqualToken::GetValue() const -> std::string { return "<="; };
}  // namespace token
