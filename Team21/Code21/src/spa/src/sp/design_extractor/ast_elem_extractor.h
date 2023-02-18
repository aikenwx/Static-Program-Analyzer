#pragma once

#include "extractor.h"

namespace design_extractor {
class AstElemExtractor : public Extractor {
 public:
  std::optional<std::vector<rel::Relationship*>> HandleBinaryOperationNode(
      std::vector<ast::INode*> parents,
      ast::BinaryOperationNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleConstantNode(
      std::vector<ast::INode*> parents, ast::ConstantNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandlePrintNode(
      std::vector<ast::INode*> parents, ast::PrintNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandleProcedureNode(
      std::vector<ast::INode*> parents, ast::ProcedureNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandleProgramNode(
      std::vector<ast::INode*> parents, ast::ProgramNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleReadNode(
      std::vector<ast::INode*> parents, ast::ReadNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandleStatementListNode(
      std::vector<ast::INode*> parents, ast::StatementListNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleStatementNode(
      std::vector<ast::INode*> parents, ast::StatementNode* node) override {
    return std::nullopt;
  };
  std::optional<std::vector<rel::Relationship*>> HandleVariableNode(
      std::vector<ast::INode*> parents, ast::VariableNode* node) override;
  static AstElemExtractor* GetInstance();
  void operator=(const AstElemExtractor&) = delete;

 private:
  AstElemExtractor() = default;
  static AstElemExtractor* instance_;
};
}  // namespace design_extractor
