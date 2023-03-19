#include "sp/ast/astlib.h"
#include "program_subparser.h"
#include "sp/token/end_token.h"
#include "util/instance_of.h"

namespace parser {
auto ProgramSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  if (context->IsLookaheadTypeOf<token::EndToken>()) {
    // program: procedure+ (adds a procedure to a program)
    if (stack->size() >= 2
      && util::instance_of<ast::ProgramNode>(*iter)
      && util::instance_of<ast::ProcedureNode>(*std::next(iter, 1))) {
      // References program node
      std::shared_ptr<ast::ProgramNode> prg =
          std::static_pointer_cast<ast::ProgramNode>(stack->back());
      // Pops program node
      stack->pop_back();
      // References procedure node
      std::shared_ptr<ast::ProcedureNode> pro =
          std::static_pointer_cast<ast::ProcedureNode>(stack->back());
      // Pops procedure node
      stack->pop_back();
      // Adds procedure to program node
      prg->AddProcedure(pro);
      // Pushes program node to parse stack
      stack->push_back(prg);
      return true;
    }
    // program: procedure+ (creates a one procedure program)
    if (util::instance_of<ast::ProcedureNode>(*iter)) {
      // References procedure node
      std::shared_ptr<ast::ProcedureNode> pro =
          std::static_pointer_cast<ast::ProcedureNode>(stack->back());
      // Pops procedure node
      stack->pop_back();
      // Creates program node
      std::shared_ptr<ast::ProgramNode> prg =
          std::make_shared<ast::ProgramNode>();
      // Adds procedure to program node
      prg->AddProcedure(pro);
      // Pushes program node to parse stack
      stack->push_back(prg);
      return true;
    }
  }
  return Subparser::Parse(context);
}
}  // namespace parser
