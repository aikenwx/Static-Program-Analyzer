#include "simple_tokenizer.h"

#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "exceptions/syntax_error.h"
#include "sp/token_factory/simple_token_factory.h"

namespace tokenizer {
using vec_sz_t = std::vector<char>::size_type;

auto SimpleTokenizer::tokenize(std::string_view program)
    -> std::vector<std::unique_ptr<token::Token>> {
  auto factory = token_factory::SimpleTokenFactory();
  std::vector<std::unique_ptr<token::Token>> tokens;

  // MSVC can't infer `const auto* iter` from const_it for some reason
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto iter = program.cbegin();
  std::vector<char> buffer;
  bool isValidSymbolPrefix = false;
  vec_sz_t lastValidSymbolLen = -1;
  auto tokenize = [&](vec_sz_t len = -1) {
    // tokenizes the buffer (up to a length of `len`)
    // then shifts the iterator towards the front as required,
    // i.e. if len != -1 or len < buffer.size()
    if (!buffer.empty()) {
      std::string_view value(buffer.data(), len != -1 ? len : buffer.size());
      tokens.push_back(factory.CreateToken(value));

      iter =
          std::prev(iter, static_cast<std::basic_string<char>::difference_type>(
                              buffer.size() - value.size()));
      buffer.clear();
      isValidSymbolPrefix = false;
      lastValidSymbolLen = -1;
    }
  };

  while (iter != program.cend()) {
    const char &chr = *iter;
    iter = std::next(iter, 1);

    if (factory.isWhitespace(chr)) {
      // if we get a whitespace, then it's definitely the end of a token
      tokenize();
      continue;
    }

    if (!isValidSymbolPrefix && factory.isSymbolPrefixChar(chr)) {
      // if this is currently not a valid symbol prefix char
      // (e.g. buffer has nothing, int, or identifier)...

      // and we get a symbol prefix char...
      // then this is the start of a new token
      tokenize();
      isValidSymbolPrefix = true;
    }

    // in any case, we'll still need the char in the buffer
    buffer.push_back(chr);

    if (!isValidSymbolPrefix) {
      // do nothing
      continue;
    }

    // else, if it *was*/is a valid symbol prefix, then check if it's still
    // one
    auto checkSymbolResult =
        factory.checkSymbol(std::string_view(buffer.data(), buffer.size()));
    if (!checkSymbolResult.isValidPrefix) {
      // if it's no longer a valid prefix...
      if (lastValidSymbolLen == -1) {
        // and there's no buffer prefix that's a valid symbol (?!?)
        // then it's probably invalid syntax
        throw exceptions::SyntaxError("Unexpected symbol");
      }

      // we have a valid symbol in the prefix
      tokenize(lastValidSymbolLen);
      // (then we continue tokenizing everything before)
      continue;
    }

    // if it's a valid symbol...
    // it could also be a valid prefix for a longer symbol too
    // update lastValidSymbolLen if applicable...
    if (checkSymbolResult.isValidSymbol) {
      lastValidSymbolLen = buffer.size();
    }

    // then continue, I guess
  }
  // otherwise there's nothing for us to do now

  // we're out of characters -- let's flush the buffer
  tokenize();

  return tokens;
}
} // namespace tokenizer
