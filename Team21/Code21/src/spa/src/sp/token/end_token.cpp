#include "end_token.h"

#include <cassert>
#include <memory>
#include <string>

namespace token {
auto EndToken::CreateToken() -> std::unique_ptr<Token> {
  return std::make_unique<EndToken>();
};

auto EndToken::GetValue() const -> std::string { return "\00"; };
} // namespace token
