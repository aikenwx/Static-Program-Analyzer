#pragma once

#include "extractor.h"

namespace design_extractor {
class DirectlyModifiesExtractor : public Extractor {
  // nb: will only handle "direct" relationships, which means that e.g.:
  // - Modifies(call, v) will never be included
  // - Modifies(proc, v) will only be included if some statement in p directly
  //   modifies v
  // - Modifies(container, v) will only be included if some statement in
  //   container directly modifies v
  // - Modifies(read, v) will always hold
  // - Modifies(assg, v) will always hold (if v is on LHS of assg)
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
      std::vector<ast::INode*> parents, ast::VariableNode* node) override {
    return std::nullopt;
  };
  static DirectlyModifiesExtractor* GetInstance();
  void operator=(const DirectlyModifiesExtractor&) = delete;

 private:
  DirectlyModifiesExtractor() = default;
  static DirectlyModifiesExtractor* instance_;
};
}  // namespace design_extractor
