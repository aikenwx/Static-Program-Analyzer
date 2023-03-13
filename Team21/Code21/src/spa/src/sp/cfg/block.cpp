#include "block.h"

namespace cfg {
Block::Block(int start, int end) {
  start_ = start;
  end_ = end;
}

int Block::start() {
  return start_;
}

int Block::end() {
  return end_;
}

bool Block::IsInBlock(int stmtNo) {
  return stmtNo >= start_ && stmtNo <= end_;
}

std::vector<std::weak_ptr<Block>> Block::parents() {
  return parents_;
}

std::vector<std::weak_ptr<Block>> Block::children() {
  return children_;
}

void Block::AddParent(std::weak_ptr<Block> parent) {
  parents_.push_back(parent);
}

void Block::AddChild(std::weak_ptr<Block> child) {
  children_.push_back(child);
}
}
