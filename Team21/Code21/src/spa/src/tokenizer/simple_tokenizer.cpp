#include "simple_tokenizer.h"

#include <deque>
#include <string>
#include <vector>

#include "../exceptions/syntax_error.h"
#include "../token_factory/simple_token_factory.h"

namespace tokenizer {
std::vector<token::Token*> SimpleTokenizer::parse(const std::string& program) {
  token_factory::SimpleTokenFactory* factory =
      token_factory::SimpleTokenFactory::getInstance();
  std::vector<token::Token*> tokens;

  std::string::const_iterator it = program.cbegin();
  std::vector<char> buffer;
  bool isValidSymbolPrefix = false;
  int lastValidSymbolLen = -1;
  auto tokenize = [&](int len = -1) {
    // tokenizes the buffer (up to a length of `len`)
    // then shifts the iterator towards the front as required,
    // i.e. if len != -1 or len < buffer.size()
    if (!buffer.empty()) {
      std::string value(buffer.begin(),
                        len != -1 ? buffer.begin() + len : buffer.end());
      tokens.push_back(factory->createToken(value));
      it -= buffer.size() - value.size();
      buffer.clear();
      isValidSymbolPrefix = false;
      lastValidSymbolLen = -1;
    }
  };

  while (it != program.cend()) {
    const char& c = *it;
    it++;

    if (factory->isWhitespace(c)) {
      // if we get a whitespace, then it's definitely the end of a token
      tokenize();
      continue;
    }

    if (!isValidSymbolPrefix) {
      // if this is currently not a valid symbol prefix char
      // (e.g. buffer has nothing, int, or identifier)
      if (factory->isSymbolPrefixChar(c)) {
        // and we get a symbol prefix char...
        // then this is the start of a new token
        tokenize();
        isValidSymbolPrefix = true;
      }
    }

    // in any case, we'll still need the char in the buffer
    buffer.push_back(c);

    // TODO: consider seeing if it's possible to just pass the vector<char>
    // into the func, to avoid instantiating a string
    if (isValidSymbolPrefix) {
      // if it *was* (or is) a valid symbol prefix, then check if it's still
      // one
      auto checkSymbolResult =
          factory->checkSymbol(std::string(buffer.begin(), buffer.end()));
      if (!checkSymbolResult.isValidPrefix) {
        // if it's no longer a valid prefix...
        if (lastValidSymbolLen == -1) {
          // and there's no buffer prefix that's a valid symbol (?!?)
          // then it's probably invalid syntax
          throw exceptions::SyntaxError("Invalid syntax");
        } else {
          // we have a valid symbol in the prefix
          tokenize(lastValidSymbolLen);
          // (then we continue tokenizing everything before)
          continue;
        }
      } else {
        // still a valid prefix
        // update lastValidSymbolLen if applicable...
        if (checkSymbolResult.isValidSymbol) {
          lastValidSymbolLen = buffer.size();
        }
        // then continue, I guess
        continue;
      }
    }
    // otherwise there's nothing for us to do now
  }

  return tokens;
}

SimpleTokenizer::SimpleTokenizer() {}

SimpleTokenizer* SimpleTokenizer::getInstance() {
  if (instance_ == nullptr) {
    instance_ = new SimpleTokenizer();
  }
  return instance_;
}

SimpleTokenizer* SimpleTokenizer::instance_ = nullptr;
}  // namespace tokenizer
