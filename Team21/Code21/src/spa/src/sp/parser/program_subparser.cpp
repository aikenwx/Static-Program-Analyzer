#include "sp/ast/astlib.h"
#include "program_subparser.h"
#include "sp/token/end_token.h"
#include "util/instance_of.h"

namespace parser {
bool ProgramSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::EndToken>()) {
    // Pr <- P Pr
    if (stack->size() >= 2
      && util::instance_of<ast::ProgramNode>(*i)
      && util::instance_of<ast::ProcedureNode>(*std::next(i, 1))) {
      // References program node
      std::shared_ptr<ast::ProgramNode> pr = std::static_pointer_cast<ast::ProgramNode>(stack->back());
      // Pops program node
      stack->pop_back();
      // References procedure node
      std::shared_ptr<ast::ProcedureNode> p = std::static_pointer_cast<ast::ProcedureNode>(stack->back());
      // Pops procedure node
      stack->pop_back();
      // Adds procedure to program node
      pr->AddProcedure(p);
      // Pushes program node to parse stack
      stack->push_back(pr);
      return true;
    }
    // Pr <- P
    if (util::instance_of<ast::ProcedureNode>(*i)) {
      // References procedure node
      std::shared_ptr<ast::ProcedureNode> p = std::static_pointer_cast<ast::ProcedureNode>(stack->back());
      // Pops procedure node
      stack->pop_back();
      // Creates program node
      std::shared_ptr<ast::ProgramNode> pr = std::make_shared<ast::ProgramNode>();
      // Adds procedure to program node
      pr->AddProcedure(p);
      // Pushes program node to parse stack
      stack->push_back(pr);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}
