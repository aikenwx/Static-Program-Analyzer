#include "block.h"

namespace cfg {
auto Block::start() const -> int { return start_; }

auto Block::end() const -> int { return end_; }

auto Block::IsInBlock(int stmtNo) const -> bool {
  return stmtNo >= start_ && stmtNo <= end_;
}

auto Block::parents() const -> std::vector<std::weak_ptr<Block>> {
  return parents_;
}

auto Block::children() const -> std::vector<std::weak_ptr<Block>> {
  return children_;
}

void Block::AddParent(const std::shared_ptr<Block>& parent) {
  AddParentOnly(parent);
  parent->AddChildOnly(shared_from_this());
}

void Block::AddChild(const std::shared_ptr<Block>& child) {
  AddChildOnly(child);
  child->AddParentOnly(shared_from_this());
}

void Block::AddParentOnly(const std::shared_ptr<Block>& parent) {
  parents_.push_back(parent);
}

void Block::AddChildOnly(const std::shared_ptr<Block>& child) {
  children_.push_back(child);
}
} // namespace cfg
