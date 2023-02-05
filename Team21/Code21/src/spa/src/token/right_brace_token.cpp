#include <assert.h>
#include <string>

#include "right_brace_token.h"

namespace token {
RightBraceToken::RightBraceToken() {};

const std::string RightBraceToken::getValue() {
    return "}";
};

const RightBraceToken* RightBraceToken::createToken(std::string str) {
    assert(str == "}");
    return new RightBraceToken();
};
}