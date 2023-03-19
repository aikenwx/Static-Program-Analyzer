#include "not_token.h"

#include <cassert>
#include <string>

namespace token {
auto NotToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "!"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<NotToken>(new NotToken());
};

auto NotToken::GetValue() const -> std::string { return "!"; };
}  // namespace token
