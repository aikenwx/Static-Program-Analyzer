#include <memory>
#include <string>
#include <vector>

#include "../token/token.h"
#include "tokenizer.h"

namespace tokenizer {
class SimpleTokenizer : public Tokenizer {
 public:
  std::vector<std::unique_ptr<token::Token>> tokenize(const std::string& program) override;
  SimpleTokenizer();
};
}  // namespace tokenizer
