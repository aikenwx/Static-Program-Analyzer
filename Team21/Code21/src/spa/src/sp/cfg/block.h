#pragma once

#include <memory>
#include <vector>

namespace cfg {
class Block {
public:
  Block(int start, int end);
  int start();
  int end();
  bool IsInBlock(int stmtNo);
  std::vector<std::weak_ptr<Block>> parents();
  std::vector<std::weak_ptr<Block>> children();
  void AddParent(std::weak_ptr<Block> parent);
  void AddChild(std::weak_ptr<Block> child);

private:
  std::vector<std::weak_ptr<Block>> parents_;
  std::vector<std::weak_ptr<Block>> children_;
  int start_;
  int end_;
};
}
