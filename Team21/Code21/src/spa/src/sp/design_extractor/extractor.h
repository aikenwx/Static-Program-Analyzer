#pragma once

#include <memory>
#include <optional>

#include "../rel/relationship.h"
#include "../ast/ast.h"
#include "../ast/assign_node.h"
#include "../ast/call_node.h"
#include "../ast/if_node.h"
#include "../ast/while_node.h"
#include "../ast/constant_node.h"
#include "../ast/print_node.h"
#include "../ast/procedure_node.h"
#include "../ast/program_node.h"
#include "../ast/read_node.h"
#include "../ast/statement_list_node.h"
#include "../ast/variable_node.h"

namespace design_extractor {
class Extractor {
 public:
  virtual ~Extractor() = default;
  virtual void HandleAssignNode(std::shared_ptr<ast::AssignNode> node, int depth) = 0;
  virtual void HandleCallNode(std::shared_ptr<ast::CallNode> node, int depth) = 0;
  virtual void HandleIfNode(std::shared_ptr<ast::IfNode> node, int depth) = 0;
  virtual void HandleWhileNode(std::shared_ptr<ast::WhileNode> node, int depth) = 0;
  virtual void HandleConstantNode(std::shared_ptr<ast::ConstantNode> node, int depth) = 0;
  virtual void HandlePrintNode(std::shared_ptr<ast::PrintNode> node, int depth) = 0;
  virtual void HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node, int depth) = 0;
  virtual void HandleProgramNode(std::shared_ptr<ast::ProgramNode> node, int depth) = 0;
  virtual void HandleReadNode(std::shared_ptr<ast::ReadNode> node, int depth) = 0;
  virtual void HandleStatementListNode(std::shared_ptr<ast::StatementListNode> node, int depth) = 0;
  virtual void HandleStatementNode(std::shared_ptr<ast::StatementNode> node, int depth) = 0;
  virtual void HandleVariableNode(std::shared_ptr<ast::VariableNode> node, int depth) = 0;
  virtual void HandleOtherNode(std::shared_ptr<ast::INode> node, int depth) = 0;
};
}  // namespace design_extractor
