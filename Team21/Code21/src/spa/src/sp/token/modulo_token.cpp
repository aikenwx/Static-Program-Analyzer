#include "modulo_token.h"

#include <cassert>
#include <string>

namespace token {
auto ModuloToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  assert(str == "%"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<ModuloToken>(new ModuloToken());
};

auto ModuloToken::GetValue() const -> std::string { return "%"; };
}  // namespace token
