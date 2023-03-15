#pragma once

#include <cassert>
#include <memory>
#include <vector>

namespace cfg {
class Block : public std::enable_shared_from_this<Block> {
public:
  Block(int start, int end) : start_(start), end_(end) {
    assert(start <= end); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  };
  auto start() const -> int;
  auto end() const -> int;
  auto IsInBlock(int stmtNo) const -> bool;
  auto parents() const -> std::vector<std::weak_ptr<Block>>;
  auto children() const -> std::vector<std::weak_ptr<Block>>;
  void AddParent(const std::shared_ptr<Block>& parent);
  void AddChild(const std::shared_ptr<Block>& child);

 private:
  std::vector<std::weak_ptr<Block>> parents_;
  std::vector<std::weak_ptr<Block>> children_;
  int start_;
  int end_;

  void AddParentOnly(const std::shared_ptr<Block>& parent);
  void AddChildOnly(const std::shared_ptr<Block>& child);
};
}  // namespace cfg
