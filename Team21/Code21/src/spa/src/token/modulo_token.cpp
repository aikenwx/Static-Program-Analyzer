#include "modulo_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> ModuloToken::CreateToken(std::string str) {
  assert(str == "%"); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return std::unique_ptr<ModuloToken>(new ModuloToken());
};

ModuloToken::ModuloToken() {};

const std::string ModuloToken::GetValue() { return "%"; };
}  // namespace token
