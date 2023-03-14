#include "cfg.h"

namespace cfg {
auto CFG::GetBlockAt(int stmtNo) -> std::optional<std::shared_ptr<Block>> {
  return intervals_.Search(stmtNo);
}

void CFG::InsertBlock(const std::shared_ptr<Block>& block) {
  intervals_.Insert({block->start(), block->end()}, block);
}

auto CFG::Size() const -> int { return intervals_.Size(); }
}  // namespace cfg
