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

  [[nodiscard]] auto cfg() const -> std::shared_ptr<cfg::CFG>;

 private:
  std::shared_ptr<cfg::CFG> cfg_ = std::make_shared<cfg::CFG>();

  auto CFGHandleStatementList(const std::vector<std::shared_ptr<cfg::Block>>& parents, const std::shared_ptr<ast::StatementListNode>& node) -> std::vector<std::shared_ptr<cfg::Block>>;
  auto CFGHandleIfStatement(const std::vector<std::shared_ptr<cfg::Block>>& parents, const std::shared_ptr<ast::IfNode>& node) -> std::vector<std::shared_ptr<cfg::Block>>;
  auto CFGHandleWhileStatement(const std::vector<std::shared_ptr<cfg::Block>>& parents, const std::shared_ptr<ast::WhileNode>& node) -> std::vector<std::shared_ptr<cfg::Block>>;
  [[nodiscard]] auto NewBlock(const std::vector<std::shared_ptr<cfg::Block>>& parents, int startStmt, int endStmt) const -> std::shared_ptr<cfg::Block>;
};
}  // namespace design_extractor
