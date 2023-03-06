#pragma once

#include <memory>
#include <vector>

#include "sp/ast/astlib.h"
#include "simple_chain_parser.h"
#include "token/token.h"

namespace parser {
class Context {
public:
  template<typename T>
  bool IsLookaheadTypeOf();
  std::shared_ptr< std::vector<std::shared_ptr<ast::INode>>> GetStack();
  int &GetStatementCounter();

private:
  friend class SimpleChainParser;
  std::vector<std::unique_ptr<token::Token>>::iterator lookahead;
  std::vector<std::shared_ptr<ast::INode>> stack;
  int statementCounter;
};
}
