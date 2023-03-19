#pragma once

#include <memory>
#include <vector>

#include "sp/ast/astlib.h"
#include "sp/token/token.h"
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
  explicit Context(std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead);
  template<typename T>
  auto IsLookaheadTypeOf() -> bool {
    return util::instance_of<T>(**lookahead);
  }
  auto GetStack() -> std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>>;
  auto GetStatementCounter() -> int &;
  auto GetLookahead() -> std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator>;

private:
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead;
  std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> stack;
  int statementCounter{};
};
}  // namespace parser
