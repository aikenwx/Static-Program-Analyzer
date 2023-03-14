#include "block.h"

namespace cfg {
int Block::start() const {
  return start_;
}

int Block::end() const {
  return end_;
}

bool Block::IsInBlock(int stmtNo) const {
  return stmtNo >= start_ && stmtNo <= end_;
}

std::vector<std::weak_ptr<Block>> Block::parents() const {
  return parents_;
}

std::vector<std::weak_ptr<Block>> Block::children() const {
  return children_;
}

void Block::AddParent(std::shared_ptr<Block> parent) {
  AddParentOnly(parent);
  parent->AddChildOnly(shared_from_this());
}

void Block::AddChild(std::shared_ptr<Block> child) {
  AddChildOnly(child);
  child->AddParentOnly(shared_from_this());
}

void Block::AddParentOnly(std::shared_ptr<Block> parent) {
  parents_.push_back(parent);
}

void Block::AddChildOnly(std::shared_ptr<Block> child) {
  children_.push_back(child);
}
} // namespace cfg
