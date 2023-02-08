#include <string>
#include <vector>

#include "token/token.h"

namespace tokenizer {
bool verifyTokenizerOutput(std::vector<token::Token*> tokens,
                           std::vector<std::string> expected) {
  if (tokens.size() != expected.size()) return false;
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i]->getValue() != expected[i]) return false;
  }
  return true;
};
}  // namespace tokenizer
