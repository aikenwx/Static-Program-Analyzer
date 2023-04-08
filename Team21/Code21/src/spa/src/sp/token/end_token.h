#pragma once

#include <memory>
#include <string>

#include "token.h"

namespace token {
/**
 * Special token denoting end of input, used in parser
 */
class EndToken : public Token {
public:
  static auto CreateToken() -> std::unique_ptr<Token>;
  [[nodiscard]] auto GetValue() const -> std::string override;
  EndToken() = default;
};
} // namespace token
