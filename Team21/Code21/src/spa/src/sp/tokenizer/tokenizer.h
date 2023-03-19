#pragma once

#include <string>
#include <vector>

#include "../token/token.h"

namespace tokenizer {
class Tokenizer {
public:
  Tokenizer() = default;
  Tokenizer(const Tokenizer &) = default;
  Tokenizer(Tokenizer &&) = delete;
  auto operator=(const Tokenizer &) -> Tokenizer & = default;
  auto operator=(Tokenizer &&) -> Tokenizer & = delete;
  virtual ~Tokenizer() = default;
  virtual auto tokenize(std::string_view program)
      -> std::vector<std::unique_ptr<token::Token>> = 0;
};
} // namespace tokenizer
