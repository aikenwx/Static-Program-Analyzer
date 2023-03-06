#pragma once

#include <memory>
#include <vector>

#include "sp/ast/astlib.h"
#include "token/token.h"

namespace parser {

class Context {
public:
  Context(std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead);
  template<typename T>
  bool IsLookaheadTypeOf();
  std::shared_ptr< std::vector<std::shared_ptr<ast::INode>>> GetStack();
  int &GetStatementCounter();
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> GetLookahead();


private:
  std::shared_ptr<std::vector<std::unique_ptr<token::Token>>::iterator> lookahead;
  std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> stack;
  int statementCounter;
};
}
