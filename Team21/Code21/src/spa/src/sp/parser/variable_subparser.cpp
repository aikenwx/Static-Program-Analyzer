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
auto VariableSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
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
    // A variable is a name
    if (util::instance_of<ast::NameNode>(*iter)) {
      // References name node for variable name
      std::shared_ptr<ast::NameNode> nam =
          std::static_pointer_cast<ast::NameNode>(stack->back());
      // Pops name node
      stack->pop_back();
      // Creates variable node
      std::shared_ptr<ast::VariableNode> var =
          std::make_shared<ast::VariableNode>(nam->GetName());
      // Pushes variable node to parse stack
      stack->push_back(var);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
