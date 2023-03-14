#include "cfg.h"

namespace cfg {
std::optional<std::shared_ptr<Block>> CFG::GetBlockAt(int stmtNo) {
  return intervals_.Search(stmtNo);
}

void CFG::InsertBlock(std::shared_ptr<Block> block) {
  intervals_.Insert({block->start(), block->end()}, block);
}

int CFG::Size() const { return intervals_.Size(); }
}  // namespace cfg
