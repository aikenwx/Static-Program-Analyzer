#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../token/token.h"
#include "tokenizer.h"

namespace tokenizer {
class SimpleTokenizer : public Tokenizer {
 public:
  auto tokenize(std::string_view program) -> std::vector<std::unique_ptr<token::Token>> override;
};
}  // namespace tokenizer
