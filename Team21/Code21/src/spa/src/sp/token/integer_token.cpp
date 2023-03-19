#include "integer_token.h"

#include <cassert>
#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_integer.h"

namespace token {
auto IntegerToken::CreateToken(std::string_view str)
    -> std::unique_ptr<Token> {
  // invariant: length > 0, all digits
  assert(util::is_integer(str)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  if (str[0] == '0' && str.length() > 1) {
    throw exceptions::SyntaxError("Non-zero integer cannot start with 0");
  }
  return std::unique_ptr<IntegerToken>(new IntegerToken(str));;
}

auto IntegerToken::GetValue() const -> std::string { return this->value; }
}  // namespace token
