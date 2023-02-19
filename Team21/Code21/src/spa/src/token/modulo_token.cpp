#include "modulo_token.h"

#include <assert.h>

#include <string>

namespace token {
const std::unique_ptr<Token> ModuloToken::createToken(std::string str) {
  assert(str == "%");
  return std::unique_ptr<ModuloToken>(new ModuloToken());
};

ModuloToken::ModuloToken() {};

const std::string ModuloToken::GetValue() { return "%"; };
}  // namespace token
