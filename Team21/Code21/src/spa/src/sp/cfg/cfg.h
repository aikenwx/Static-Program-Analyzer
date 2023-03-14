#pragma once

#include <memory>

#include "../../util/interval_tree.h"
#include "block.h"

namespace cfg {
class CFG {
 public:
  CFG() = default;
  auto GetBlockAt(int stmtNo) -> std::optional<std::shared_ptr<Block>>;
  void InsertBlock(const std::shared_ptr<Block>& block);

  [[nodiscard]] auto Size() const -> int;

 private:
  util::IntervalTree<int, std::shared_ptr<Block>> intervals_;
};
}  // namespace cfg
