#pragma once

#include <memory>

#include "block.h"
#include "../../util/interval_tree.h"

namespace cfg {
class CFG {
public:
  CFG();
  std::optional<std::shared_ptr<Block>> GetBlockAt(int stmtNo);
  void InsertBlock(std::shared_ptr<Block> block);

private:
  util::IntervalTree<int, std::shared_ptr<Block>> intervals_;
};
}
