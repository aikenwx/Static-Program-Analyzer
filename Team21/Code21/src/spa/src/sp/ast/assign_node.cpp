#include "assign_node.h"

namespace ast {
AssignNode::AssignNode(std::shared_ptr<VariableNode> var,
                       std::shared_ptr<ExpressionNode> exp) {
  this->var = var;
  this->assignment = exp->GetOperand();
}

std::shared_ptr<VariableNode> AssignNode::GetVariable() { return var; }

std::shared_ptr<INode> AssignNode::GetAssignment() { return assignment; }

std::string AssignNode::ToString() const {
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
