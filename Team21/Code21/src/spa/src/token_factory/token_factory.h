#include <string>

namespace token_factory {
typedef struct CheckSymbolResult {
  bool isValidPrefix;
  bool isValidSymbol;
} CheckSymbolResult;
class TokenFactory {
 public:
  virtual token::Token* createToken(std::string value) = 0;
  virtual bool isWhitespace(const char& c) = 0;
  virtual bool isSymbolPrefixChar(const char& c) = 0;
  virtual CheckSymbolResult checkSymbol(const std::string& value) = 0;
  static TokenFactory* getInstance();
  void operator=(const TokenFactory&) = delete;

 private:
  static TokenFactory* instance_;
};
};  // namespace token_factory
