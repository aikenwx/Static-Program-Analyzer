#pragma once

#include <string>
#include <vector>

#include "../token/token.h"

namespace tokenizer {
class Tokenizer {
 public:
  virtual std::vector<std::unique_ptr<token::Token>> tokenize(const std::string& program) = 0;
};
}  // namespace tokenizer
