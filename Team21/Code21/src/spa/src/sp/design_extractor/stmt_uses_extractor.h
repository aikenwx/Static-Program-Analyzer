#pragma once

#include "../ast/assign_node.h"
#include "../ast/call_node.h"
#include "../ast/constant_node.h"
#include "../ast/if_node.h"
#include "../ast/print_node.h"
#include "../ast/procedure_node.h"
#include "../ast/program_node.h"
#include "../ast/statement_list_node.h"
#include "../ast/statement_node.h"
#include "../ast/read_node.h"
#include "../ast/variable_node.h"
#include "../ast/while_node.h"
#include "../rel/uses_stmt_var_relationship.h"
#include "extractor.h"

namespace design_extractor {
class StmtUsesExtractor : public Extractor {
  // nb: will only handle statement-uses relns, which means that e.g.:
  // - Uses(call, v) will not be included here, but should be derived in SP
  //   facade based on Uses(proc, v)
  // - Uses(proc, v) will not be included here, but should be derived in SP
  //   facade based on proc stmt numbers and Uses(stmt, v)
  // - Uses(container, v) will not be included here, but should be derived
  //   in SP facade based on Parent()
  // - Uses(print, v) will always hold
  // - Uses(assg, v) will always hold (if v is on RHS of assg)
 public:
  StmtUsesExtractor() = default;
  void HandleAssignNode(const std::shared_ptr<ast::AssignNode>& node,
                        int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleCallNode(

      const std::shared_ptr<ast::CallNode>& node, int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleIfNode(

      const std::shared_ptr<ast::IfNode>& node, int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleWhileNode(const std::shared_ptr<ast::WhileNode>& node,
                       int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleConstantNode(const std::shared_ptr<ast::ConstantNode>& node,
                          int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandlePrintNode(const std::shared_ptr<ast::PrintNode>& node,
                       int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleProcedureNode(const std::shared_ptr<ast::ProcedureNode>& node,
                           int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleProgramNode(const std::shared_ptr<ast::ProgramNode>& node,
                         int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleReadNode(

      const std::shared_ptr<ast::ReadNode>& node, int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleStatementListNode(

      const std::shared_ptr<ast::StatementListNode>& node, int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleStatementNode(const std::shared_ptr<ast::StatementNode>& node,
                           int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };
  void HandleVariableNode(const std::shared_ptr<ast::VariableNode>& node,
                          int depth) override;
  void HandleOtherNode(const std::shared_ptr<ast::INode>& node, int depth) override {
    UpdateParents(std::static_pointer_cast<ast::INode>(node), depth);
  };

  auto GetRelationships() const -> std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>>;

 private:
  std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>> relns_;
  std::vector<std::shared_ptr<ast::INode>> parents_;
  int depth_ = -1;

  void UpdateParents(const std::shared_ptr<ast::INode>& node, int depth);
};
}  // namespace design_extractor
