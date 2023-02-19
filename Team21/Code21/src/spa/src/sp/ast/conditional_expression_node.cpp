#include "conditional_expression_node.h"

namespace ast {
ConditionalExpressionNode::ConditionalExpressionNode(std::shared_ptr<INode> operand) {
	this->operand = operand;
}

std::shared_ptr<INode> ConditionalExpressionNode::GetOperand() {
	return operand;
}

std::string ConditionalExpressionNode::ToString() const {
	return "cond_expression:" + operand->ToString() + "\n";
}
}
