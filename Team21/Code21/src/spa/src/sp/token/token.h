#pragma once
#include <memory>
#include <string>

namespace token {
class Token {
 public:
  static auto CreateToken(std::string_view) -> std::unique_ptr<Token>;
  Token() = default;
  virtual ~Token() = default;
  Token(const Token&) = delete;
  Token(Token&&) = delete;
  auto operator=(const Token&) -> Token& = delete;
  auto operator=(Token&&) -> Token& = delete
  ;
  [[nodiscard]] virtual auto GetValue() const -> std::string = 0;
};
}  // namespace token
