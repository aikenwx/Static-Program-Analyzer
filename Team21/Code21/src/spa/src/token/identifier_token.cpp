#include "identifier_token.h"

#include <assert.h>

#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_identifier.h"

namespace token {
const std::unique_ptr<Token> IdentifierToken::createToken(
    std::string str) {
  assert(str.length() > 0);  // invariant: non-zero length
  if (!util::is_identifier(str)) {
    throw exceptions::SyntaxError("Invalid identifier");
  }
  return std::unique_ptr<IdentifierToken>(new IdentifierToken(str));;
}

IdentifierToken::IdentifierToken(std::string value) { this->value = value; }

const std::string IdentifierToken::GetValue() { return this->value; }
}  // namespace token
