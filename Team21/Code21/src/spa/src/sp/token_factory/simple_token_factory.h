#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../token/identifier_token.h"
#include "token_factory.h"

namespace token_factory {
class SimpleTokenFactory : public TokenFactory {
 public:
  auto CreateToken(std::string_view value) -> std::unique_ptr<token::Token> override;
  auto isWhitespace(const char& chr) -> bool override;
  auto isSymbolPrefixChar(const char& chr) -> bool override;
  auto checkSymbol(std::string_view value) -> CheckSymbolResult override;

 private:
  static const std::unordered_map<char, std::vector<std::string>> kSymbolTokens;
  static const std::unordered_set<char> kWhitespaceTokens;
};
}  // namespace token_factory
