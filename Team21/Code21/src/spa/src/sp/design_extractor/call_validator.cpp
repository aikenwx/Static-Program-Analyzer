#include "call_validator.h"

#include <cassert>

#include "exceptions/semantic_error.h"
#include "sp/ast/call_node.h"
#include "sp/ast/program_node.h"

namespace design_extractor {
void CallValidator::HandleCallNode(const std::shared_ptr<ast::CallNode>& node, int depth) {
  assert(this->program); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  if (!(node->GetProcedure())) {
    if (program->ContainsProcedure(node->GetProcedureName())) {
      node->SetProcedure(program->GetProcedure(node->GetProcedureName()));
    } else {
      // Not sure what type of error to throw here
      throw exceptions::SemanticError("Call node cannot call procedure that does not exist");
    }
  }
}
void CallValidator::HandleProgramNode(const std::shared_ptr<ast::ProgramNode>& node, int depth) {
  this->program = node;
}
}  // namespace design_extractor
