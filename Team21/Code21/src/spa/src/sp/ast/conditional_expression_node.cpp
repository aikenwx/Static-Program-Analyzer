#include "conditional_expression_node.h"

namespace ast {
ConditionalExpressionNode::ConditionalExpressionNode(INode *operand) {
	this->operand = operand;
}

INode *ConditionalExpressionNode::GetOperand() {
	return operand;
}

std::string ConditionalExpressionNode::ToString() const {
	return "cond_expression:" + operand->ToString() + "\n";
}
}
