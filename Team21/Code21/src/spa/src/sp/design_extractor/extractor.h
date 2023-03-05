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
                                int depth){};
  virtual void HandleCallNode(std::shared_ptr<ast::CallNode> node, int depth){};
  virtual void HandleIfNode(std::shared_ptr<ast::IfNode> node, int depth){};
  virtual void HandleWhileNode(std::shared_ptr<ast::WhileNode> node,
                               int depth){};
  virtual void HandleConstantNode(std::shared_ptr<ast::ConstantNode> node,
                                  int depth){};
  virtual void HandlePrintNode(std::shared_ptr<ast::PrintNode> node,
                               int depth){};
  virtual void HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node,
                                   int depth){};
  virtual void HandleProgramNode(std::shared_ptr<ast::ProgramNode> node,
                                 int depth){};
  virtual void HandleReadNode(std::shared_ptr<ast::ReadNode> node, int depth){};
  virtual void HandleStatementListNode(
      std::shared_ptr<ast::StatementListNode> node, int depth){};
  virtual void HandleStatementNode(std::shared_ptr<ast::StatementNode> node,
                                   int depth){};
  virtual void HandleVariableNode(std::shared_ptr<ast::VariableNode> node,
                                  int depth){};
  virtual void HandleOtherNode(std::shared_ptr<ast::INode> node, int depth){};
};
}  // namespace design_extractor
