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
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleAssignNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::AssignNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleCallNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::CallNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleIfNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::IfNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleWhileNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::WhileNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleConstantNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ConstantNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandlePrintNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::PrintNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProcedureNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProcedureNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProgramNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProgramNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleReadNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ReadNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementListNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementListNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementNode> node) = 0;
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleVariableNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::VariableNode> node) = 0;
};
}  // namespace design_extractor
