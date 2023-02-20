#include "identifier_token.h"

#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_identifier.h"

namespace token {
const std::unique_ptr<Token> IdentifierToken::CreateToken(
    std::string str) {
  if (!util::is_identifier(str)) { // checks for zero-length str
    throw exceptions::SyntaxError("Invalid identifier");
  }
  return std::unique_ptr<IdentifierToken>(new IdentifierToken(str));;
}

IdentifierToken::IdentifierToken(std::string value) { this->value = value; }

const std::string IdentifierToken::GetValue() { return this->value; }
}  // namespace token
