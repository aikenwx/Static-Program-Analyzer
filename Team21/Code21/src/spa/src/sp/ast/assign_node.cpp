#include "assign_node.h"

#include <utility>

namespace ast {
AssignNode::AssignNode(std::shared_ptr<VariableNode> var,
                       const std::shared_ptr<ExpressionNode>& exp)
    : var(std::move(std::move(var))) {
  this->assignment = exp->GetOperand();
}

auto AssignNode::GetVariable() -> std::shared_ptr<VariableNode> { return var; }

auto AssignNode::GetAssignment() -> std::shared_ptr<INode> {
  return assignment;
}

auto AssignNode::ToString() const -> std::string {
  return "assign:\n{\n" + var->ToString() + assignment->ToString() + "}\n";
}

void AssignNode::AcceptVisitor(
    std::shared_ptr<INode> currentNode,
    std::shared_ptr<design_extractor::Extractor> extractor, int depth) {
  extractor->HandleStatementNode(std::static_pointer_cast<StatementNode>(currentNode),
                                 depth);
  extractor->HandleAssignNode(std::static_pointer_cast<AssignNode>(currentNode),
                              depth);

  var->AcceptVisitor(var, extractor, depth + 1);
  assignment->AcceptVisitor(assignment, extractor, depth + 1);
}
}  // namespace ast
