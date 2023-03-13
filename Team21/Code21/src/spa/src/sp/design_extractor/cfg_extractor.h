#include <stack>

#include "../cfg/block.h"
#include "../cfg/cfg.h"
#include "extractor.h"

namespace design_extractor {
class CFGExtractor : public Extractor {
 public:
  CFGExtractor() = default;

  void HandleProcedureNode(std::shared_ptr<ast::ProcedureNode> node,
                           int depth) override;

  std::shared_ptr<cfg::CFG> cfg() const;

 private:
  std::shared_ptr<cfg::CFG> cfg_ = std::make_shared<cfg::CFG>();

  std::vector<std::shared_ptr<cfg::Block>> CFGHandleStatementList(const std::vector<std::shared_ptr<cfg::Block>>& parents, std::shared_ptr<ast::StatementListNode> node);
  std::vector<std::shared_ptr<cfg::Block>> CFGHandleIfStatement(const std::vector<std::shared_ptr<cfg::Block>>& parents, std::shared_ptr<ast::IfNode> node);
  std::vector<std::shared_ptr<cfg::Block>> CFGHandleWhileStatement(const std::vector<std::shared_ptr<cfg::Block>>& parents, std::shared_ptr<ast::WhileNode> node);
  std::shared_ptr<cfg::Block> NewBlock(const std::vector<std::shared_ptr<cfg::Block>>& parents, int startStmt, int endStmt) const;
};
}  // namespace design_extractor
