#pragma once

#include <memory>
#include <optional>

#include "../rel/relationship.h"
#include "../ast/ast.h"
#include "../ast/binary_operation_node.h"
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
  virtual std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleBinaryOperationNode(std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::BinaryOperationNode> node) = 0;
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
