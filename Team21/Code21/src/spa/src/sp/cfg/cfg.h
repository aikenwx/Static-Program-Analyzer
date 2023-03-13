#pragma once

#include <memory>

#include "../../util/interval_tree.h"
#include "block.h"

namespace cfg {
class CFG {
 public:
  CFG();
  std::optional<std::shared_ptr<Block>> GetBlockAt(int stmtNo);
  void InsertBlock(std::shared_ptr<Block> block);

  int Size() const;

 private:
  util::IntervalTree<int, std::shared_ptr<Block>> intervals_;
};
}  // namespace cfg
