#include "call_validator.h"

#include <assert.h>
#include "sp/ast/call_node.h"
#include "sp/ast/program_node.h"
#include "exceptions/semantic_error.h"

namespace design_extractor {
void CallValidator::HandleCallNode(std::shared_ptr<ast::CallNode> node, int depth) {
  assert(this->program);
  if (!(node->GetProcedure())) {
    if (program->ContainsProcedure(node->GetProcedureName())) {
      node->SetProcedure(program->GetProcedure(node->GetProcedureName()));
    } else {
      // Not sure what type of error to throw here
      throw exceptions::SemanticError("Call node cannot call procedure that does not exist");
    }
  }
}
void CallValidator::HandleProgramNode(std::shared_ptr<ast::ProgramNode> node, int depth) {
  this->program = node;
}
}
