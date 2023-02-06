#include <vector>

#include "token/token.h"

namespace tokenizer {
bool verifyTokenizerOutput(std::vector<token::Token*> tokens,
                           std::vector<std::string> expected);
}  // namespace tokenizer
