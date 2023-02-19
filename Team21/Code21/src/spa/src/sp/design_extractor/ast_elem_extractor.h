#pragma once

#include "extractor.h"

namespace design_extractor {
class AstElemExtractor : public Extractor {
 public:
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleAssignNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::AssignNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleCallNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::CallNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleIfNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::IfNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleWhileNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::WhileNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleContainerStatementNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ContainerStatementNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleConstantNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ConstantNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandlePrintNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::PrintNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProcedureNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProcedureNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProgramNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProgramNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleReadNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ReadNode> node) override;
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementListNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementListNode> node) override {
    return std::nullopt;
  }
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleVariableNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::VariableNode> node) override;
  AstElemExtractor() = default;
};
}  // namespace design_extractor
