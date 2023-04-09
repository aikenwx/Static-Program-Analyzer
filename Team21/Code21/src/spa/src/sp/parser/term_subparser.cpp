#include "term_subparser.h"
#include "sp/ast/astlib.h"
#include "sp/ast/constant_node.h"
#include "sp/ast/identifier_node.h"
#include "sp/ast/multiplicative_operation_node.h"
#include "sp/ast/symbol_node.h"
#include "util/instance_of.h"
#include "util/is_symbol_node_value.h"

namespace parser {
// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
auto TermSubparser::Parse(std::shared_ptr<Context> context) -> bool {
  auto stack = context->GetStack();
  auto iter = stack->rbegin();
  auto is_correct_symbol =
      [&](const std::shared_ptr<ast::INode> &node) {
        return IsSymbolNodeValue(node, "*") ||
               IsSymbolNodeValue(node, "/") ||
               IsSymbolNodeValue(node, "%");
      };
  if (context->IsLookaheadSymbolValueAnyOf({")", ";", "+", "-", "*", "/", "%",
                                            "<", ">",
                                            "==", "<=", ">=", "!="})) {
    // term: term ['*', '/', '%'] factor
    if (stack->size() >= 3 && util::instance_of<ast::IdentifierNode>(*iter) &&
        is_correct_symbol(*std::next(iter, 1)) &&
        util::instance_of<ast::TermNode>(*std::next(iter, 2))) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // References multiplicative symbol type
      std::string sym =
          std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops multiplicative symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> ter1 =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates multiplicative operation node
      std::shared_ptr<ast::MultiplicativeOperationNode> bin =
          std::make_shared<ast::MultiplicativeOperationNode>(ter1->GetOperand(),
                                                             var, sym);
      // Creates term node
      std::shared_ptr<ast::TermNode> ter2 =
          std::make_shared<ast::TermNode>(bin);
      // Pushes term node to parse stack
      stack->push_back(ter2);
      return true;
    }
    if (stack->size() >= 3 && util::instance_of<ast::ConstantNode>(*iter) &&
        is_correct_symbol(*std::next(iter, 1)) &&
        util::instance_of<ast::TermNode>(*std::next(iter, 2))) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> con =
          std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pops constant node
      stack->pop_back();
      // References multiplicative symbol type
      std::string sym =
          std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops multiplicative symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> ter1 =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates multiplicative operation node
      std::shared_ptr<ast::MultiplicativeOperationNode> bin =
          std::make_shared<ast::MultiplicativeOperationNode>(ter1->GetOperand(),
                                                             con, sym);
      // Creates term node
      std::shared_ptr<ast::TermNode> ter2 =
          std::make_shared<ast::TermNode>(bin);
      // Pushes term node to parse stack
      stack->push_back(ter2);
      return true;
    }
    if (IsSymbolNodeValue(*iter, ")") &&
        util::instance_of<ast::ExpressionNode>(*std::next(iter, 1)) &&
        IsSymbolNodeValue(*std::next(iter, 2), "(") &&
        is_correct_symbol(*std::next(iter, 3)) &&
        util::instance_of<ast::TermNode>(*std::next(iter, 4))) {
      // Pops right paren symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // References multiplicative symbol type
      std::string sym =
          std::static_pointer_cast<ast::SymbolNode>(stack->back())->GetType();
      // Pops multiplicative symbol node
      stack->pop_back();
      // References term node
      std::shared_ptr<ast::TermNode> ter1 =
          std::static_pointer_cast<ast::TermNode>(stack->back());
      // Pops term node
      stack->pop_back();
      // Creates multiplicative operation node
      std::shared_ptr<ast::MultiplicativeOperationNode> bin =
          std::make_shared<ast::MultiplicativeOperationNode>(
              ter1->GetOperand(), exp->GetOperand(), sym);
      // Creates term node
      std::shared_ptr<ast::TermNode> ter2 =
          std::make_shared<ast::TermNode>(bin);
      // Pushes term node to parse stack
      stack->push_back(ter2);
      return true;
    }
    // term: factor
    if (util::instance_of<ast::IdentifierNode>(*iter)) {
      // References identifier node
      std::shared_ptr<ast::IdentifierNode> var =
          std::static_pointer_cast<ast::IdentifierNode>(stack->back());
      // Pops identifier node
      stack->pop_back();
      // Creates term node
      std::shared_ptr<ast::TermNode> ter = std::make_shared<ast::TermNode>(var);
      // Pushes term node to parse stack
      stack->push_back(ter);
      return true;
    }
    if (util::instance_of<ast::ConstantNode>(*iter)) {
      // References constant node
      std::shared_ptr<ast::ConstantNode> con =
          std::static_pointer_cast<ast::ConstantNode>(stack->back());
      // Pops constant node
      stack->pop_back();
      // Creates term node
      std::shared_ptr<ast::TermNode> ter = std::make_shared<ast::TermNode>(con);
      // Pushes term node to parse stack
      stack->push_back(ter);
      return true;
    }
    if (IsSymbolNodeValue(*iter, ")") &&
        util::instance_of<ast::ExpressionNode>(*std::next(iter, 1)) &&
        IsSymbolNodeValue(*std::next(iter, 2), "(")) {
      // Pops right paren symbol node
      stack->pop_back();
      // References expression node
      std::shared_ptr<ast::ExpressionNode> exp =
          std::static_pointer_cast<ast::ExpressionNode>(stack->back());
      // Pops expression node
      stack->pop_back();
      // Pops left paren symbol node
      stack->pop_back();
      // Creates term node
      std::shared_ptr<ast::TermNode> ter =
          std::make_shared<ast::TermNode>(exp->GetOperand());
      // Pushes term node to parse stack
      stack->push_back(ter);
      return true;
    }
  }
  return Subparser::Parse(context);
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
} // namespace parser
