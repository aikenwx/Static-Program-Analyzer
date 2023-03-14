#include "sp/ast/astlib.h"
#include "variable_subparser.h"
#include "sp/token/assign_token.h"
#include "sp/token/semicolon_token.h"
#include "sp/token/right_paren_token.h"
#include "sp/token/plus_token.h"
#include "sp/token/minus_token.h"
#include "sp/token/multiply_token.h"
#include "sp/token/divide_token.h"
#include "sp/token/modulo_token.h"
#include "sp/token/less_than_token.h"
#include "sp/token/greater_than_token.h"
#include "sp/token/equal_token.h"
#include "sp/token/less_equal_token.h"
#include "sp/token/greater_equal_token.h"
#include "sp/token/not_equal_token.h"
#include "util/instance_of.h"

namespace parser {
bool VariableSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::AssignToken>()
    || context->IsLookaheadTypeOf<token::SemicolonToken>()
    || context->IsLookaheadTypeOf<token::RightParenToken>()
    || context->IsLookaheadTypeOf<token::PlusToken>()
    || context->IsLookaheadTypeOf<token::MinusToken>()
    || context->IsLookaheadTypeOf<token::MultiplyToken>()
    || context->IsLookaheadTypeOf<token::DivideToken>()
    || context->IsLookaheadTypeOf<token::ModuloToken>()
    || context->IsLookaheadTypeOf<token::LessThanToken>()
    || context->IsLookaheadTypeOf<token::GreaterThanToken>()
    || context->IsLookaheadTypeOf<token::EqualToken>()
    || context->IsLookaheadTypeOf<token::LessEqualToken>()
    || context->IsLookaheadTypeOf<token::GreaterEqualToken>()
    || context->IsLookaheadTypeOf<token::NotEqualToken>()) {
    // V <- N
    if (util::instance_of<ast::NameNode>(*i)) {
      // References name node for variable name
      std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack->back());
      // Pops name node
      stack->pop_back();
      // Creates variable node
      std::shared_ptr<ast::VariableNode> v = std::make_shared<ast::VariableNode>(n->GetName());
      // Pushes variable node to parse stack
      stack->push_back(v);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
