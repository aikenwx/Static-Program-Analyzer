#include "modulo_token.h"

#include <assert.h>

#include <string>

namespace token {
ModuloToken::ModuloToken() {};

const std::string ModuloToken::getValue() { return "%"; };

const ModuloToken* ModuloToken::createToken(std::string str) {
  assert(str == "%");
  if (instance_ == nullptr) instance_ = new ModuloToken();
  return instance_;
};

ModuloToken* ModuloToken::instance_ = nullptr;
}  // namespace token
