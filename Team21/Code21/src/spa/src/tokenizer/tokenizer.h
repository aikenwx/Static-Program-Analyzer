#include <string>
#include <vector>

#include "../token/token.h"

namespace tokenizer {
class Tokenizer {
 public:
  virtual std::vector<token::Token*> parse(const std::string& program) = 0;
  static Tokenizer* getInstance();
  void operator=(const Tokenizer&) = delete;

 private:
  static Tokenizer* instance_;  
};
}  // namespace tokenizer
