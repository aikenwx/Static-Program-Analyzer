#include <string>
#include <vector>

#include "../token/token.h"
#include "tokenizer.h"

namespace tokenizer {
class SimpleTokenizer : public Tokenizer {
 public:
  std::vector<token::Token*> tokenize(const std::string& program) override;
  static SimpleTokenizer* getInstance();
 private:
  SimpleTokenizer();
  static SimpleTokenizer* instance_;
};
}  // namespace tokenizer
