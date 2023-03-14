#pragma once

#include <vector>

#include "../rel/relationship.h"
#include "extractor.h"

namespace design_extractor {
class AstElemExtractor : public Extractor {
 public:
  AstElemExtractor() = default;
  void HandleAssignNode(const std::shared_ptr<ast::AssignNode>& node,
                        int depth) override;
  void HandleCallNode(const std::shared_ptr<ast::CallNode>& node, int depth) override;
  void HandleIfNode(const std::shared_ptr<ast::IfNode>& node, int depth) override;
  void HandleWhileNode(const std::shared_ptr<ast::WhileNode>& node,
                       int depth) override;
  void HandleConstantNode(const std::shared_ptr<ast::ConstantNode>& node,
                          int depth) override;
  void HandlePrintNode(const std::shared_ptr<ast::PrintNode>& node,
                       int depth) override;
  void HandleProcedureNode(const std::shared_ptr<ast::ProcedureNode>& node,
                           int depth) override;
  void HandleReadNode(const std::shared_ptr<ast::ReadNode>& node, int depth) override;
  void HandleVariableNode(const std::shared_ptr<ast::VariableNode>& node,
                          int depth) override;

  std::vector<std::shared_ptr<rel::Relationship>> GetRelationships() const;

 private:
  std::vector<std::shared_ptr<rel::Relationship>> relns_;
  std::shared_ptr<ast::ProcedureNode> currentProc_;
};
}  // namespace design_extractor
