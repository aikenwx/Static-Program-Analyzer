#include "sp/ast/astlib.h"
#include "assign_subparser.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
bool AssignSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::SemicolonToken>()) {
    // assign: var_name '=' expr ';'
    if (stack->size() >= 3
      && util::instance_of<ast::ExpressionNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kAssign
      && util::instance_of<ast::VariableNode>(*std::next(i, 2))) {
      // References expression node
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Pops assign symbol node
      stack->pop_back();
      // References variable node
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack->back());
      // Pops variable node
      stack->pop_back();
      // Creates assign node
      std::shared_ptr<ast::AssignNode> a = std::make_shared<ast::AssignNode>(v, e);
      context->GetStatementCounter()++;
      // Pushes assign node to parse stack
      stack->push_back(a);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
