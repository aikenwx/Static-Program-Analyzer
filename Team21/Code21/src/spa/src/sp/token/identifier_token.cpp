#include "identifier_token.h"

#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_identifier.h"

namespace token {
auto IdentifierToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  if (!util::is_identifier(str)) { // checks for zero-length str
    throw exceptions::SyntaxError("Invalid identifier");
  }
  return std::unique_ptr<IdentifierToken>(new IdentifierToken(str));;
}

auto IdentifierToken::GetValue() const -> std::string { return this->value; }
}  // namespace token
