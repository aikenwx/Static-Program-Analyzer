#include "sp/ast/astlib.h"
#include "program_subparser.h"
#include "sp/ast/procedure_node.h"
#include "sp/token/end_token.h"
#include "util/instance_of.h"

namespace parser {
auto ProgramSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  // program: procedure+ (adds a procedure to a program)
  if (stack->size() >= 2
    && util::instance_of<ast::ProcedureNode>(*iter)
    && util::instance_of<ast::ProgramNode>(*std::next(iter, 1))) {
    // References procedure node
    std::shared_ptr<ast::ProcedureNode> pro =
        std::static_pointer_cast<ast::ProcedureNode>(stack->back());
    // Pops procedure node
    stack->pop_back();
    // References program node
    std::shared_ptr<ast::ProgramNode> prg =
        std::static_pointer_cast<ast::ProgramNode>(stack->back());
    // Adds procedure to program node
    prg->AddProcedure(pro);
    // Maintains program node at top of stack
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
  return Subparser::Parse(context);
}
}  // namespace parser
