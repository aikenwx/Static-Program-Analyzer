#include "integer_token.h"

#include <assert.h>

#include <string>

#include "../exceptions/syntax_error.h"
#include "../util/is_integer.h"
#include "integer_token.h"

namespace token {
IntegerToken::IntegerToken(std::string value) { this->value = value; }

const std::string IntegerToken::getValue() { return this->value; }

const IntegerToken* IntegerToken::createToken(std::string str) {
  assert(util::is_integer(str)); // invariant: length > 0, all digits
  if (str[0] == '0' && str.length() > 1) {
    throw exceptions::SyntaxError("Non-zero integer cannot start with 0");
  }
  return new IntegerToken(str);
}
}  // namespace token
