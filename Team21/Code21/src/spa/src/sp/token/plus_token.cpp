#include "plus_token.h"

#include <cassert>
#include <string>

namespace token {
auto PlusToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "+"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<PlusToken>(new PlusToken());
};

auto PlusToken::GetValue() const -> std::string { return "+"; };
}  // namespace token
