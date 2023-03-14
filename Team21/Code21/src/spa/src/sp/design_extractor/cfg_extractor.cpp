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

std::shared_ptr<cfg::Block> CFGExtractor::NewBlock(const std::vector<std::shared_ptr<cfg::Block>>& parents, int startStmt, int endStmt) const {
  auto newBlock = std::make_shared<cfg::Block>(startStmt, endStmt);
  cfg_->InsertBlock(newBlock);
  for (auto parent : parents) {
    newBlock->AddParent(parent);
    // child automatically added
  }
  return newBlock;
}

std::vector<std::shared_ptr<cfg::Block>> CFGExtractor::CFGHandleStatementList(
    const std::vector<std::shared_ptr<cfg::Block>>& parents,
    std::shared_ptr<ast::StatementListNode> node) {
  std::vector<std::shared_ptr<cfg::Block>> newParents = parents;

  int startStmt = -1;
  int endStmt = -1;

  for (auto statement : node->GetStatements()) {
    if (startStmt == -1) {
      startStmt = statement->GetStatementNumber();
    }

    if (util::instance_of<ast::IfNode>(statement)) {
      if (endStmt != -1) {
        newParents = {NewBlock(newParents, startStmt, endStmt)};
        endStmt = -1;
      }

      newParents = CFGHandleIfStatement(
          newParents, std::static_pointer_cast<ast::IfNode>(statement));
      startStmt = -1;
    } else if (util::instance_of<ast::WhileNode>(statement)) {
      if (endStmt != -1) {
        newParents = {NewBlock(newParents, startStmt, endStmt)};
        endStmt = -1;
      }

      newParents = CFGHandleWhileStatement(
          newParents, std::static_pointer_cast<ast::WhileNode>(statement));
      startStmt = -1;
    } else {
      endStmt = statement->GetStatementNumber();
    }
  }

  if (endStmt != -1) {
    newParents = {NewBlock(newParents, startStmt, endStmt)};
  }

  return newParents;
}

std::vector<std::shared_ptr<cfg::Block>> CFGExtractor::CFGHandleIfStatement(
    const std::vector<std::shared_ptr<cfg::Block>>& parents,
    std::shared_ptr<ast::IfNode> node) {
  auto condBlock = NewBlock(parents, node->GetStatementNumber(),
                            node->GetStatementNumber());

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
  auto condBlock = NewBlock(parents, node->GetStatementNumber(),
                            node->GetStatementNumber());

  auto bodyBlocks = CFGHandleStatementList({condBlock}, node->GetBody());

  return bodyBlocks;
}

std::shared_ptr<cfg::CFG> CFGExtractor::cfg() const { return cfg_; }
}  // namespace design_extractor
