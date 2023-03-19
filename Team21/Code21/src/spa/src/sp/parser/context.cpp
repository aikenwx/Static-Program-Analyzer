#include "context.h"

#include <utility>

namespace parser {
Context::Context(std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead) : lookahead(std::move(lookahead)) {
  this->stack = std::make_shared<std::vector<std::shared_ptr<ast::INode>>>();
}

auto Context::GetStack() -> std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> {
  return stack;
}

auto Context::GetStatementCounter() -> int & {
  return statementCounter;
}

auto Context::GetLookahead() -> std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> {
  return lookahead;
}
}  // namespace parser
