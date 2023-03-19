#include "or_token.h"

#include <cassert>
#include <string>

namespace token {
auto OrToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "||"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<OrToken>(new OrToken());
};

auto OrToken::GetValue() const -> std::string { return "||"; };
}  // namespace token
