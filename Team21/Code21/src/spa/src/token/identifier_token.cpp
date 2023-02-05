#include "identifier_token.h"

#include <assert.h>
#include <string>

#include "exceptions/syntax_error.h"
#include "util/is_identifier.h"

namespace token {
IdentifierToken::IdentifierToken(std::string value) {
    this->value = value;
}

const std::string IdentifierToken::getValue() {
    return this->value;
}

const IdentifierToken* IdentifierToken::createToken(std::string str) {
    assert(str.length() > 0); // invariant: non-zero length
    if (!util::is_identifier(str)) {
        throw exceptions::SyntaxError("Invalid identifier");
    }
    return new IdentifierToken(str);
}
}  // namespace token
