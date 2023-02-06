#pragma once

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
  virtual std::optional<std::vector<rel::Relationship>> HandleBinaryOperationNode(std::vector<ast::INode*> parents, ast::BinaryOperationNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleConstantNode(std::vector<ast::INode*> parents, ast::ConstantNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandlePrintNode(std::vector<ast::INode*> parents, ast::PrintNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleProcedureNode(std::vector<ast::INode*> parents, ast::ProcedureNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleProgramNode(std::vector<ast::INode*> parents, ast::ProgramNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleReadNode(std::vector<ast::INode*> parents, ast::ReadNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleStatementListNode(std::vector<ast::INode*> parents, ast::StatementListNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleStatementNode(std::vector<ast::INode*> parents, ast::StatementNode* node) = 0;
  virtual std::optional<std::vector<rel::Relationship>> HandleVariableNode(std::vector<ast::INode*> parents, ast::VariableNode* node) = 0;
};
}  // namespace design_extractor
