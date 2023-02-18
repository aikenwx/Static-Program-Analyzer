#pragma once

#include "extractor.h"

namespace design_extractor {
class ParentExtractor : public Extractor {
 public:
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleBinaryOperationNode(
      std::vector<std::shared_ptr<ast::INode>> parents,
      std::shared_ptr<ast::BinaryOperationNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleConstantNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ConstantNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandlePrintNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::PrintNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProcedureNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProcedureNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleProgramNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ProgramNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleReadNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::ReadNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementListNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementListNode> node) override {
    return std::nullopt;
  }
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleStatementNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::StatementNode> node) override {
    return std::nullopt;
  };
  std::optional<std::vector<std::unique_ptr<rel::Relationship>>> HandleVariableNode(
      std::vector<std::shared_ptr<ast::INode>> parents, std::shared_ptr<ast::VariableNode> node) override {
    return std::nullopt;
  };
  static ParentExtractor* GetInstance();
  void operator=(const ParentExtractor&) = delete;

 private:
  ParentExtractor() = default;
  static ParentExtractor* instance_;
};
}  // namespace design_extractor
