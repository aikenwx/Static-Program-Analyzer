#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../token/identifier_token.h"
#include "token_factory.h"

namespace token_factory {
class SimpleTokenFactory : public TokenFactory {
 public:
  std::unique_ptr<token::Token> CreateToken(std::string_view value) override;
  bool isWhitespace(const char& c) override;
  bool isSymbolPrefixChar(const char& c) override;
  CheckSymbolResult checkSymbol(const std::string& value) override;
  SimpleTokenFactory();

 private:
  static const std::unordered_map<char, std::vector<std::string>> kSymbolTokens;
  static const std::unordered_set<char> kWhitespaceTokens;
};
}  // namespace token_factory
