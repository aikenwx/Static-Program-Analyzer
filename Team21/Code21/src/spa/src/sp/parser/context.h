#pragma once

#include <memory>
#include <vector>

#include "sp/ast/astlib.h"
#include "token/token.h"
#include "util/instance_of.h"

namespace parser {

class Context {
public:
  Context(std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead);
  template<typename T>
  bool IsLookaheadTypeOf() {
    return util::instance_of<T>(**lookahead);
  }
  std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> GetStack();
  int &GetStatementCounter();
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> GetLookahead();


private:
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead;
  std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> stack;
  int statementCounter;
};
}
