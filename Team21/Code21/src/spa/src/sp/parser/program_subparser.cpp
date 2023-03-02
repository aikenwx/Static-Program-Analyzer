#include "sp/ast/astlib.h"
#include "program_subparser.h"
#include "token/end_token.h"
#include "util/instance_of.h"

namespace parser {
bool ProgramSubparser::Parse(std::shared_ptr<Context> context) {
  if (context->IsLookaheadTypeOf<token::EndToken>()) {
    auto stack = context->GetStack();
    auto i = stack->rbegin();
    if (stack->size() >= 2
      && util::instance_of<ast::ProgramNode>(*i)
      && util::instance_of<ast::ProcedureNode>(*std::next(i, 1))) {
      // Pr <- P Pr
      std::shared_ptr<ast::ProgramNode> pr = std::static_pointer_cast<ast::ProgramNode>(stack->back());
      stack->pop_back();
      std::shared_ptr<ast::ProcedureNode> p = std::static_pointer_cast<ast::ProcedureNode>(stack->back());
      stack->pop_back();
      pr->AddProcedure(p);
      stack->push_back(pr);
      return true;
    }
  } else {
    return Subparser::Parse(context);
  }
}
}
