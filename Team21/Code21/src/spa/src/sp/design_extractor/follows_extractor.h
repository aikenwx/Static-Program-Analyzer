#pragma once

#include <optional>

#include "extractor.h"

namespace design_extractor {
class FollowsExtractor : public Extractor {
 public:
  std::optional<std::vector<rel::Relationship*>> HandleBinaryOperationNode(
      std::vector<ast::INode*> parents,
      ast::BinaryOperationNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleConstantNode(
      std::vector<ast::INode*> parents, ast::ConstantNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandlePrintNode(
      std::vector<ast::INode*> parents, ast::PrintNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleProcedureNode(
      std::vector<ast::INode*> parents, ast::ProcedureNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleProgramNode(
      std::vector<ast::INode*> parents, ast::ProgramNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleReadNode(
      std::vector<ast::INode*> parents, ast::ReadNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleStatementListNode(
      std::vector<ast::INode*> parents, ast::StatementListNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandleStatementNode(
      std::vector<ast::INode*> parents, ast::StatementNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleVariableNode(
      std::vector<ast::INode*> parents, ast::VariableNode* node) override {
    return std::nullopt;
  };
  static FollowsExtractor* GetInstance();
  void operator=(const FollowsExtractor&) = delete;

 private:
  FollowsExtractor() = default;
  static FollowsExtractor* instance_;
};
}  // namespace design_extractor