#include "sp/token/token.h"
#include <memory>
#include <string>

namespace token_factory {
using CheckSymbolResult = struct CheckSymbolResult {
  bool isValidPrefix;
  bool isValidSymbol;
};
class TokenFactory {
public:
  TokenFactory() = default;
  TokenFactory(const TokenFactory &) = delete;
  TokenFactory(TokenFactory &&) = delete;
  auto operator=(const TokenFactory &) -> TokenFactory & = delete;
  auto operator=(TokenFactory &&) -> TokenFactory & = delete;
  virtual ~TokenFactory() = default;
  virtual auto CreateToken(const std::string& value) -> std::unique_ptr<token::Token> = 0;
  virtual auto isWhitespace(const char& chr) -> bool = 0;
  virtual auto isSymbolPrefixChar(const char& chr) -> bool = 0;
  virtual auto checkSymbol(std::string_view chr) -> CheckSymbolResult = 0;
};
}; // namespace token_factory
