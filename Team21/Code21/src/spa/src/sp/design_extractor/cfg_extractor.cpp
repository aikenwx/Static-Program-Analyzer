#include "cfg_extractor.h"

#include <assert.h>

#include "../../util/instance_of.h"
#include "../ast/if_node.h"
#include "../ast/procedure_node.h"
#include "../ast/while_node.h"

namespace design_extractor {
// doing this recursively for better understandability
// plus, possibly not very OCP-ish

void CFGExtractor::HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node,
                                       int depth) {
  CFGHandleStatementList({}, node->GetStatements());
}

std::vector<std::shared_ptr<cfg::Block>> CFGExtractor::CFGHandleStatementList(
    const std::vector<std::shared_ptr<cfg::Block>>& parents,
    std::shared_ptr<ast::StatementListNode> node) {
  std::vector<std::shared_ptr<cfg::Block>> new_parents = parents;

  int startStmt = -1;
  int endStmt = -1;

  for (auto statement : node->GetStatements()) {
    if (startStmt == -1) {
      startStmt = statement->GetStatementNumber();
    }

    if (util::instance_of<ast::IfNode>(statement)) {
      if (endStmt != -1) {
        auto newBlock = std::make_shared<cfg::Block>(startStmt, endStmt);
        cfg_->InsertBlock(newBlock);
        for (auto parent : parents) {
          newBlock->AddParent(parent);
          parent->AddChild(newBlock);
        }
        new_parents = {newBlock};
        endStmt = -1;
      }

      new_parents = CFGHandleIfStatement(
          parents, std::static_pointer_cast<ast::IfNode>(statement));
      startStmt = -1;
    } else if (util::instance_of<ast::WhileNode>(statement)) {
      if (endStmt != -1) {
        auto newBlock = std::make_shared<cfg::Block>(startStmt, endStmt);
        cfg_->InsertBlock(newBlock);
        for (auto parent : parents) {
          newBlock->AddParent(parent);
          parent->AddChild(newBlock);
        }
        new_parents = {newBlock};
        endStmt = -1;
      }

      new_parents = CFGHandleWhileStatement(
          parents, std::static_pointer_cast<ast::WhileNode>(statement));
      startStmt = -1;
    } else {
      endStmt = statement->GetStatementNumber();
    }
  }

  if (endStmt != -1) {
    auto newBlock = std::make_shared<cfg::Block>(startStmt, endStmt);
    cfg_->InsertBlock(newBlock);
    for (auto parent : parents) {
      newBlock->AddParent(parent);
      parent->AddChild(newBlock);
    }
    new_parents = {newBlock};
  }

  return new_parents;
}

std::vector<std::shared_ptr<cfg::Block>> CFGExtractor::CFGHandleIfStatement(
    const std::vector<std::shared_ptr<cfg::Block>>& parents,
    std::shared_ptr<ast::IfNode> node) {
  auto condBlock = std::make_shared<cfg::Block>(node->GetStatementNumber(),
                                                node->GetStatementNumber());
  for (auto parent : parents) {
    condBlock->AddParent(parent);
    parent->AddChild(condBlock);
  }

  auto thenBlocks = CFGHandleStatementList({condBlock}, node->GetThen());
  auto elseBlocks = CFGHandleStatementList({condBlock}, node->GetElse());

  std::vector<std::shared_ptr<cfg::Block>> endBlocks;
  endBlocks.reserve(thenBlocks.size() + elseBlocks.size());
  endBlocks.insert(endBlocks.end(), thenBlocks.begin(), thenBlocks.end());
  endBlocks.insert(endBlocks.end(), elseBlocks.begin(), elseBlocks.end());
  return endBlocks;
}

std::vector<std::shared_ptr<cfg::Block>> CFGExtractor::CFGHandleWhileStatement(
    const std::vector<std::shared_ptr<cfg::Block>>& parents,
    std::shared_ptr<ast::WhileNode> node) {
  auto condBlock = std::make_shared<cfg::Block>(node->GetStatementNumber(),
                                                node->GetStatementNumber());
  for (auto parent : parents) {
    condBlock->AddParent(parent);
    parent->AddChild(condBlock);
  }

  auto bodyBlocks = CFGHandleStatementList({condBlock}, node->GetBody());

  return bodyBlocks;
}

std::shared_ptr<cfg::CFG> CFGExtractor::cfg() const { return cfg_; }
}  // namespace design_extractor
