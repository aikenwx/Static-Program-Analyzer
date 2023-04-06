#pragma once

#include <memory>
#include <vector>

#include "sp/ast/astlib.h"
#include "sp/token/token.h"
#include "sp/token/util.h"
#include "util/instance_of.h"

namespace parser {

class Context {
public:
  Context() = default;
  Context(const Context &) = delete;
  Context(Context &&) = delete;
  auto operator=(const Context &) -> Context & = delete;
  auto operator=(Context &&) -> Context & = delete;
  virtual ~Context() = default;
  explicit Context(
      std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator>
          lookahead);
  template <typename T> [[nodiscard]] auto IsLookaheadTypeOf() const -> bool {
    return util::instance_of<T>(**lookahead);
  }
  [[nodiscard]] auto IsLookaheadSymbolValue(std::string_view value) const
      -> bool {
    return token::IsTokenSymbolValue(**lookahead, value);
  }
  [[nodiscard]] auto IsLookaheadSymbolValueAnyOf(
      std::initializer_list<std::string_view> values) const -> bool {
    return token::IsTokenSymbolValueAnyOf(**lookahead, values);
  }
  auto GetStack() -> std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>>;
  auto GetStatementCounter() -> int &;
  auto GetLookahead()
      -> std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator>;

private:
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator>
      lookahead;
  std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> stack;
  int statementCounter{};
};
} // namespace parser
