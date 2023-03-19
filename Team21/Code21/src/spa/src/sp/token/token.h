#pragma once
#include <memory>
#include <string>

namespace token {
enum TokenType {
  AND,
  ASSIGN,
  DIVIDE,
  END, // special token denoting end of input, used in parser
  EQUAL,
  GREATER_EQUAL,
  GREATER_THAN,
  IDENTIFIER,
  INTEGER,
  LEFT_BRACE,
  LEFT_PAREN,
  LESS_EQUAL,
  LESS_THAN,
  MINUS,
  MODULO,
  MULTIPLY,
  NOT_EQUAL,
  NOT,
  OR,
  PLUS,
  RIGHT_BRACE,
  RIGHT_PAREN,
  SEMICOLON,
};

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
  [[nodiscard]] virtual auto GetType() const -> TokenType = 0;
  [[nodiscard]] virtual auto GetValue() const -> std::string = 0;
};
}  // namespace token
