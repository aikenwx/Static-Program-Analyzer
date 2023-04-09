#pragma once

#include <memory>
#include <optional>

namespace ast {
class INode;
class AssignNode;
class CallNode;
class ConditionalNode;
class IdentifierNode;
class IfNode;
class WhileNode;
class ConstantNode;
class PrintNode;
class ProcedureNode;
class ProgramNode;
class ReadNode;
class StatementListNode;
class StatementNode;
}  // namespace ast

namespace design_extractor {
class Extractor {
 public:
  Extractor() = default;
  Extractor(const Extractor&) = delete;
  Extractor(Extractor&&) = delete;
  auto operator=(const Extractor&) -> Extractor& = delete;
  auto operator=(Extractor&&) -> Extractor& = delete;
  virtual ~Extractor() = default;

  // default implementations included for convenience
  virtual void HandleAssignNode(const std::shared_ptr<ast::AssignNode>& node,
                                int depth){};
  virtual void HandleCallNode(const std::shared_ptr<ast::CallNode>& node, int depth){};
  virtual void HandleConditionalNode(const std::shared_ptr<ast::ConditionalNode>& node,
                                     int depth){};
  virtual void HandleIfNode(const std::shared_ptr<ast::IfNode>& node, int depth){};
  virtual void HandleWhileNode(const std::shared_ptr<ast::WhileNode>& node,
                               int depth){};
  virtual void HandleConstantNode(const std::shared_ptr<ast::ConstantNode>& node,
                                  int depth){};
  virtual void HandlePrintNode(const std::shared_ptr<ast::PrintNode>& node,
                               int depth){};
  virtual void HandleProcedureNode(const std::shared_ptr<ast::ProcedureNode>& node,
                                   int depth){};
  virtual void HandleProgramNode(const std::shared_ptr<ast::ProgramNode>& node,
                                 int depth){};
  virtual void HandleReadNode(const std::shared_ptr<ast::ReadNode>& node, int depth){};
  virtual void HandleStatementListNode(
      const std::shared_ptr<ast::StatementListNode>& node, int depth){};
  virtual void HandleStatementNode(const std::shared_ptr<ast::StatementNode>& node,
                                   int depth){};
  virtual void HandleVariableNode(const std::shared_ptr<ast::IdentifierNode>& node,
                                  int depth){};
  virtual void HandleOtherNode(const std::shared_ptr<ast::INode>& node, int depth){};
};
}  // namespace design_extractor
