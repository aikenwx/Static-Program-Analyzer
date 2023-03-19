#include "multiply_token.h"

#include <cassert>
#include <string>

namespace token {
auto MultiplyToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "*"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<MultiplyToken>(new MultiplyToken());
};

auto MultiplyToken::GetValue() const -> std::string { return "*"; };
}  // namespace token
