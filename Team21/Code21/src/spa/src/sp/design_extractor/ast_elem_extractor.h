#pragma once

#include <vector>

#include "../rel/relationship.h"
#include "extractor.h"

namespace design_extractor {
class AstElemExtractor : public Extractor {
 public:
  AstElemExtractor() = default;
  void HandleAssignNode(std::shared_ptr<ast::AssignNode> node,
                        int depth) override;
  void HandleCallNode(std::shared_ptr<ast::CallNode> node, int depth) override;
  void HandleIfNode(std::shared_ptr<ast::IfNode> node, int depth) override;
  void HandleWhileNode(std::shared_ptr<ast::WhileNode> node,
                       int depth) override;
  void HandleConstantNode(std::shared_ptr<ast::ConstantNode> node,
                          int depth) override;
  void HandlePrintNode(std::shared_ptr<ast::PrintNode> node,
                       int depth) override;
  void HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node,
                           int depth) override;
  void HandleReadNode(std::shared_ptr<ast::ReadNode> node, int depth) override;
  void HandleVariableNode(std::shared_ptr<ast::VariableNode> node,
                          int depth) override;

  std::vector<std::shared_ptr<rel::Relationship>> GetRelationships() const;

 private:
  std::vector<std::shared_ptr<rel::Relationship>> relns_;
  std::shared_ptr<ast::ProcedureNode> currentProc_;
};
}  // namespace design_extractor
