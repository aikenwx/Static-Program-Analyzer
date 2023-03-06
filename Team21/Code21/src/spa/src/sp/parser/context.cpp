#include "context.h"
#include "util/instance_of.h"

namespace parser {
Context::Context(std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead) {
  this->stack = std::make_shared<std::vector<std::shared_ptr<ast::INode>>>();
  this->lookahead = lookahead;
}

template<typename T>
bool Context::IsLookaheadTypeOf() {
  return util::instance_of<T>(**lookahead);
}

std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> Context::GetStack() {
  return std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>>(stack);
}

int &Context::GetStatementCounter() {
  return statementCounter;
}

std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> Context::GetLookahead() {
  return lookahead;
}
}
