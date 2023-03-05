#pragma once

#include <memory>
#include <optional>

namespace ast {
class INode;
class AssignNode;
class CallNode;
class IfNode;
class WhileNode;
class ConstantNode;
class PrintNode;
class ProcedureNode;
class ProgramNode;
class ReadNode;
class StatementListNode;
class StatementNode;
class VariableNode;
}  // namespace ast

namespace design_extractor {
class Extractor {
 public:
  virtual ~Extractor() = default;
  virtual void HandleAssignNode(std::shared_ptr<ast::AssignNode> node,
                                int depth) = 0;
  virtual void HandleCallNode(std::shared_ptr<ast::CallNode> node, int depth) = 0;
  virtual void HandleIfNode(std::shared_ptr<ast::IfNode> node, int depth) = 0;
  virtual void HandleWhileNode(std::shared_ptr<ast::WhileNode> node,
                               int depth) = 0;
  virtual void HandleConstantNode(std::shared_ptr<ast::ConstantNode> node,
                                  int depth) = 0;
  virtual void HandlePrintNode(std::shared_ptr<ast::PrintNode> node,
                               int depth) = 0;
  virtual void HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node,
                                   int depth) = 0;
  virtual void HandleProgramNode(std::shared_ptr<ast::ProgramNode> node,
                                 int depth) = 0;
  virtual void HandleReadNode(std::shared_ptr<ast::ReadNode> node, int depth) = 0;
  virtual void HandleStatementListNode(
      std::shared_ptr<ast::StatementListNode> node, int depth) = 0;
  virtual void HandleStatementNode(std::shared_ptr<ast::StatementNode> node,
                                   int depth) = 0;
  virtual void HandleVariableNode(std::shared_ptr<ast::VariableNode> node,
                                  int depth) = 0;
  virtual void HandleOtherNode(std::shared_ptr<ast::INode> node, int depth) = 0;
};
}  // namespace design_extractor
