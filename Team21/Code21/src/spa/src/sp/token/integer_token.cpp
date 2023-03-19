#include "integer_token.h"

#include <assert.h>

#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_integer.h"
#include "integer_token.h"

namespace token {
const std::unique_ptr<Token> IntegerToken::CreateToken(std::string str) {
  assert(util::is_integer(str)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)  // invariant: length > 0, all digits
  if (str[0] == '0' && str.length() > 1) {
    throw exceptions::SyntaxError("Non-zero integer cannot start with 0");
  }
  return std::unique_ptr<IntegerToken>(new IntegerToken(str));;
}

IntegerToken::IntegerToken(std::string value) { this->value = value; }

const std::string IntegerToken::GetValue() { return this->value; }
}  // namespace token
