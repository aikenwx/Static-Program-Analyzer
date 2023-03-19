#include "and_token.h"

#include <cassert>
#include <string>

namespace token {
auto AndToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "&&"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<AndToken>(new AndToken());
};

auto AndToken::GetValue() const -> std::string { return "&&"; };
}  // namespace token
