#pragma once

#include <memory>
#include <vector>

namespace cfg {
class Block {
public:
  Block(int start, int end);
  int start() const;
  int end() const;
  bool IsInBlock(int stmtNo) const;
  std::vector<std::weak_ptr<Block>> parents() const;
  std::vector<std::weak_ptr<Block>> children() const;
  void AddParent(std::weak_ptr<Block> parent);
  void AddChild(std::weak_ptr<Block> child);

private:
  std::vector<std::weak_ptr<Block>> parents_;
  std::vector<std::weak_ptr<Block>> children_;
  int start_;
  int end_;
};
}
