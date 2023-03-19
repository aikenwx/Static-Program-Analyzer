#include <memory>
#include <string>

namespace token_factory {
typedef struct CheckSymbolResult {
  bool isValidPrefix;
  bool isValidSymbol;
} CheckSymbolResult;
class TokenFactory {
 public:
  virtual std::unique_ptr<token::Token> CreateToken(std::string value) = 0;
  virtual bool isWhitespace(const char& c) = 0;
  virtual bool isSymbolPrefixChar(const char& c) = 0;
  virtual CheckSymbolResult checkSymbol(const std::string& value) = 0;
};
};  // namespace token_factory
