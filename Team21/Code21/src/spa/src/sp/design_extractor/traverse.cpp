#include "extractor.h"
#include "util/instance_of.h"

namespace design_extractor {
std::vector<rel::Relationship*> _Traverse(
    std::vector<ast::INode*> parents, ast::INode* node,
    design_extractor::Extractor* extractor) {
  std::vector<rel::Relationship*> relationships;
  if (util::instance_of<ast::ProgramNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleProgramNode(parents,
                                     static_cast<ast::ProgramNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    for (auto procedure :
         *static_cast<ast::ProgramNode*>(node)->GetProcedures()) {
      std::vector<rel::Relationship*> res =
          _Traverse(parents, procedure, extractor);
      relationships.insert(relationships.end(), res.begin(), res.end());
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ProcedureNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleProcedureNode(parents,
                                       static_cast<ast::ProcedureNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<rel::Relationship*> res = _Traverse(
        parents, static_cast<ast::ProcedureNode*>(node)->GetStatements(),
        extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::StatementListNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleStatementListNode(
            parents, static_cast<ast::StatementListNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    for (auto statement :
         *static_cast<ast::StatementListNode*>(node)->GetStatements()) {
      std::vector<rel::Relationship*> res =
          _Traverse(parents, statement, extractor);
      relationships.insert(relationships.end(), res.begin(), res.end());
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ReadNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleReadNode(parents, static_cast<ast::ReadNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // it's also a statement node
    newRels = extractor->HandleStatementNode(parents,
                                             static_cast<ast::ReadNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<rel::Relationship*> res = _Traverse(
        parents, static_cast<ast::ReadNode*>(node)->GetVariable(), extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::BinaryOperationNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleBinaryOperationNode(
            parents, static_cast<ast::BinaryOperationNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<rel::Relationship*> res = _Traverse(
        parents, static_cast<ast::BinaryOperationNode*>(node)->GetLeft(),
        extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    res = _Traverse(parents,
                    static_cast<ast::BinaryOperationNode*>(node)->GetRight(),
                    extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::ConstantNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleConstantNode(parents,
                                      static_cast<ast::ConstantNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // no more children to recurse into
  } else if (util::instance_of<ast::PrintNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandlePrintNode(parents, static_cast<ast::PrintNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // it's also a statement node
    newRels = extractor->HandleStatementNode(
        parents, static_cast<ast::PrintNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<rel::Relationship*> res = _Traverse(
        parents, static_cast<ast::PrintNode*>(node)->GetVariable(), extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::VariableNode>(node)) {
    std::optional<std::vector<rel::Relationship*>> newRels =
        extractor->HandleVariableNode(parents,
                                      static_cast<ast::VariableNode*>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // no more children to recurse into
  } else {
    // including abstract types, like StatementNode
    // should never happen tho :)
    throw std::runtime_error("Unknown node type");
  }

  return relationships;
}

std::vector<rel::Relationship*> Traverse(
    ast::INode* node, design_extractor::Extractor* extractor) {
  std::vector<ast::INode*> parents;
  return _Traverse(parents, node, extractor);
}

}  // namespace design_extractor
