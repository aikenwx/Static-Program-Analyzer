#include "sp/ast/astlib.h"
#include "variable_subparser.h"
#include "token/assign_token.h"
#include "token/semicolon_token.h"
#include "token/right_paren_token.h"
#include "token/plus_token.h"
#include "token/minus_token.h"
#include "token/multiply_token.h"
#include "token/divide_token.h"
#include "token/modulo_token.h"
#include "token/less_than_token.h"
#include "token/greater_than_token.h"
#include "token/equal_token.h"
#include "token/less_equal_token.h"
#include "token/greater_equal_token.h"
#include "token/not_equal_token.h"
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
    if (util::instance_of<ast::NameNode>(*i)) {
      // V <- N
      std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::VariableNode> v = std::make_shared<ast::VariableNode>(n->GetName());
      stack->push_back(v);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
