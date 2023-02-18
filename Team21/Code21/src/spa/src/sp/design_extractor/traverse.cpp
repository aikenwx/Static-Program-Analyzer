#include "extractor.h"
#include "util/instance_of.h"

namespace design_extractor {
std::vector<std::unique_ptr<rel::Relationship>> _Traverse(
    std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::INode> node,
    std::shared_ptr<design_extractor::Extractor> extractor) {
  std::vector<std::unique_ptr<rel::Relationship>> relationships;
  if (util::instance_of<ast::ProgramNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleProgramNode(parents,
                                     std::static_pointer_cast<ast::ProgramNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    for (auto procedure :
         std::static_pointer_cast<ast::ProgramNode>(node)->GetProcedures()) {
      std::vector<std::unique_ptr<rel::Relationship>> res =
          _Traverse(parents, procedure, extractor);
      relationships.insert(relationships.end(), res.begin(), res.end());
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ProcedureNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleProcedureNode(parents,
                                       std::static_pointer_cast<ast::ProcedureNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<std::unique_ptr<rel::Relationship>> res = _Traverse(
        parents, std::static_pointer_cast<ast::ProcedureNode>(node)->GetStatements(),
        extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::StatementListNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleStatementListNode(
            parents, std::static_pointer_cast<ast::StatementListNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    for (auto statement :
         std::static_pointer_cast<ast::StatementListNode>(node)->GetStatements()) {
      std::vector<std::unique_ptr<rel::Relationship>> res =
          _Traverse(parents, statement, extractor);
      relationships.insert(relationships.end(), res.begin(), res.end());
    }
    parents.pop_back();
  } else if (util::instance_of<ast::ReadNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleReadNode(parents, std::static_pointer_cast<ast::ReadNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // it's also a statement node
    newRels = extractor->HandleStatementNode(parents,
                                             std::static_pointer_cast<ast::ReadNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<std::unique_ptr<rel::Relationship>> res = _Traverse(
        parents, std::static_pointer_cast<ast::ReadNode>(node)->GetVariable(), extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::BinaryOperationNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleBinaryOperationNode(
            parents, std::static_pointer_cast<ast::BinaryOperationNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<std::unique_ptr<rel::Relationship>> res = _Traverse(
        parents, std::static_pointer_cast<ast::BinaryOperationNode>(node)->GetLeft(),
        extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    res = _Traverse(parents,
                    std::static_pointer_cast<ast::BinaryOperationNode>(node)->GetRight(),
                    extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::ConstantNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleConstantNode(parents,
                                      std::static_pointer_cast<ast::ConstantNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // no more children to recurse into
  } else if (util::instance_of<ast::PrintNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandlePrintNode(parents, std::static_pointer_cast<ast::PrintNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    // it's also a statement node
    newRels = extractor->HandleStatementNode(
        parents, std::static_pointer_cast<ast::PrintNode>(node));
    if (newRels.has_value()) {
      relationships.insert(relationships.end(), newRels.value().begin(),
                           newRels.value().end());
    }

    parents.push_back(node);
    std::vector<std::unique_ptr<rel::Relationship>> res = _Traverse(
        parents, std::static_pointer_cast<ast::PrintNode>(node)->GetVariable(), extractor);
    relationships.insert(relationships.end(), res.begin(), res.end());
    parents.pop_back();
  } else if (util::instance_of<ast::VariableNode>(node)) {
    std::optional<std::vector<std::unique_ptr<rel::Relationship>>> newRels =
        extractor->HandleVariableNode(parents,
                                      std::static_pointer_cast<ast::VariableNode>(node));
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

std::vector<std::unique_ptr<rel::Relationship>> Traverse(
    std::shared_ptr<ast::INode> node, std::shared_ptr<design_extractor::Extractor> extractor) {
  std::vector<std::shared_ptr<ast::INode>> parents;
  return _Traverse(parents, node, extractor);
}

}  // namespace design_extractor
