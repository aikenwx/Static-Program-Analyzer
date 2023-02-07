#pragma once

#include "extractor.h"

namespace design_extractor {
class DirectlyUsesExtractor : public Extractor {
  // nb: will only handle "direct" relationships, which means that e.g.:
  // - Uses(call, v) will never be included
  // - Uses(proc, v) will only be included if some statement in p directly uses
  //   v
  // - Uses(container, v) will only be included if some statement in container
  //   directly uses v
  // - Uses(print, v) will always hold
  // - Uses(assg, v) will always hold (if v is on RHS of assg)
 public:
  std::optional<std::vector<rel::Relationship*>> HandleBinaryOperationNode(
      std::vector<ast::INode*> parents,
      ast::BinaryOperationNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleConstantNode(
      std::vector<ast::INode*> parents, ast::ConstantNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandlePrintNode(
      std::vector<ast::INode*> parents, ast::PrintNode* node) override;
  std::optional<std::vector<rel::Relationship*>> HandleProcedureNode(
      std::vector<ast::INode*> parents, ast::ProcedureNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleProgramNode(
      std::vector<ast::INode*> parents, ast::ProgramNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleReadNode(
      std::vector<ast::INode*> parents, ast::ReadNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleStatementListNode(
      std::vector<ast::INode*> parents, ast::StatementListNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleStatementNode(
      std::vector<ast::INode*> parents, ast::StatementNode* node) override {
    return std::nullopt;
  }
  std::optional<std::vector<rel::Relationship*>> HandleVariableNode(
      std::vector<ast::INode*> parents, ast::VariableNode* node) override {
    return std::nullopt;
  }
  static DirectlyUsesExtractor* GetInstance();
  void operator=(const DirectlyUsesExtractor&) = delete;

 private:
  DirectlyUsesExtractor() = default;
  static DirectlyUsesExtractor* instance_;
};
}  // namespace design_extractor
