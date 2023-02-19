#include "extractor.h"
#include "sp/ast/binary_operation_node.h"
#include "sp/ast/unary_operation_node.h"
#include "util/instance_of.h"

namespace design_extractor {
bool appendNewRelationships(
    std::vector<std::unique_ptr<rel::Relationship>>& relationships,
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels) {
  if (newRels.has_value()) {
    relationships.insert(relationships.end(),
                         std::make_move_iterator(newRels.value().begin()),
                         std::make_move_iterator(newRels.value().end()));
    return true;
  }
  return false;
}

bool appendNewRelationships(
    std::vector<std::unique_ptr<rel::Relationship>>& relationships,
    std::vector<std::unique_ptr<rel::Relationship>> newRels) {
  relationships.insert(relationships.end(),
                       std::make_move_iterator(newRels.begin()),
                       std::make_move_iterator(newRels.end()));
}

std::vector<std::unique_ptr<rel::Relationship>> _Traverse(
    std::vector<std::shared_ptr<ast::INode>> parents,
    std::shared_ptr<ast::INode> node,
    std::shared_ptr<design_extractor::Extractor> extractor) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;

  // TODO: this needs a bit of deduplicatin' and cleanup

  // handle all statement nodes
  if (util::instance_of<ast::StatementNode>(node)) {
    std::shared_ptr<ast::StatementNode> stmtNode =
        std::static_pointer_cast<ast::StatementNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleStatementNode(parents, stmtNode));
  }

  // handle specific cases
  if (util::instance_of<ast::ProgramNode>(node)) {
    std::shared_ptr<ast::ProgramNode> programNode =
        std::static_pointer_cast<ast::ProgramNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleProgramNode(parents, programNode));

    parents.push_back(node);
    for (auto procedure : programNode->GetProcedures()) {
      appendNewRelationships(relationships,
                             _Traverse(parents, procedure, extractor));
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ProcedureNode>(node)) {
    std::shared_ptr<ast::ProcedureNode> procNode =
        std::static_pointer_cast<ast::ProcedureNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleProcedureNode(parents, procNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships,
        _Traverse(parents, procNode->GetStatements(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::StatementListNode>(node)) {
    std::shared_ptr<ast::StatementListNode> stmtListNode =
        std::static_pointer_cast<ast::StatementListNode>(node);

    appendNewRelationships(relationships, extractor->HandleStatementListNode(
                                              parents, stmtListNode));

    parents.push_back(node);
    for (auto statement : stmtListNode->GetStatements()) {
      appendNewRelationships(relationships,
                             _Traverse(parents, statement, extractor));
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ReadNode>(node)) {
    std::shared_ptr<ast::ReadNode> readNode =
        std::static_pointer_cast<ast::ReadNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleReadNode(parents, readNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, readNode->GetVariable(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::BinaryOperationNode>(node)) {
    std::shared_ptr<ast::BinaryOperationNode> binOpNode =
        std::static_pointer_cast<ast::BinaryOperationNode>(node);

    parents.push_back(node);
    appendNewRelationships(relationships,
                           _Traverse(parents, binOpNode->GetLeft(), extractor));
    appendNewRelationships(
        relationships, _Traverse(parents, binOpNode->GetRight(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::ConstantNode>(node)) {
    std::shared_ptr<ast::ConstantNode> constNode =
        std::static_pointer_cast<ast::ConstantNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleConstantNode(parents, constNode));
    // no more children to recurse into
  } else if (util::instance_of<ast::PrintNode>(node)) {
    std::shared_ptr<ast::PrintNode> printNode =
        std::static_pointer_cast<ast::PrintNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandlePrintNode(parents, printNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, printNode->GetVariable(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::VariableNode>(node)) {
    std::shared_ptr<ast::VariableNode> varNode =
        std::static_pointer_cast<ast::VariableNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleVariableNode(parents, varNode));
    // no more children to recurse into
  } else if (util::instance_of<ast::AssignNode>(node)) {
    std::shared_ptr<ast::AssignNode> assignNode =
        std::static_pointer_cast<ast::AssignNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleAssignNode(parents, assignNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships,
        _Traverse(parents, assignNode->GetVariable(), extractor));
    appendNewRelationships(
        relationships,
        _Traverse(parents, assignNode->GetAssignment(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::IfNode>(node)) {
    std::shared_ptr<ast::IfNode> ifNode =
        std::static_pointer_cast<ast::IfNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleIfNode(parents, ifNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, ifNode->GetCondition(), extractor));
    appendNewRelationships(
        relationships, _Traverse(parents, ifNode->GetThen(), extractor));
    appendNewRelationships(
        relationships, _Traverse(parents, ifNode->GetElse(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::WhileNode>(node)) {
    std::shared_ptr<ast::WhileNode> whileNode =
        std::static_pointer_cast<ast::WhileNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleWhileNode(parents, whileNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, whileNode->GetCondition(), extractor));
    appendNewRelationships(
        relationships, _Traverse(parents, whileNode->GetBody(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::CallNode>(node)) {
    std::shared_ptr<ast::CallNode> callNode =
        std::static_pointer_cast<ast::CallNode>(node);

    appendNewRelationships(relationships,
                           extractor->HandleCallNode(parents, callNode));

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, callNode->GetProcedure(), extractor));
    parents.pop_back();
  } else if (util::instance_of<ast::UnaryOperationNode>(node)) {
    std::shared_ptr<ast::UnaryOperationNode> unOpNode =
        std::static_pointer_cast<ast::UnaryOperationNode>(node);

    parents.push_back(node);
    appendNewRelationships(
        relationships, _Traverse(parents, unOpNode->GetOperand(), extractor));
    parents.pop_back();
  } else {
    // including abstract types, like StatementNode
    // should never happen tho :)
    throw std::runtime_error("Unknown node type");
  }

  return relationships;
}

std::vector<std::unique_ptr<rel::Relationship>> Traverse(
    std::shared_ptr<ast::INode> node,
    std::shared_ptr<design_extractor::Extractor> extractor) {
  std::vector<std::shared_ptr<ast::INode>> parents;
  return _Traverse(parents, node, extractor);
}

}  // namespace design_extractor
