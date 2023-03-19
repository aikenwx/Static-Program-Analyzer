#include "end_token.h"

#include <cassert>
#include <memory>
#include <string>

namespace token {
auto EndToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "$"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::make_unique<EndToken>();
};

auto EndToken::GetValue() const -> std::string { return "$"; };
}  // namespace token
