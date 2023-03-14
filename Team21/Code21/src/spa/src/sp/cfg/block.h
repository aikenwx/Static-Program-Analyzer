#pragma once

#include <memory>
#include <vector>

namespace cfg {
class Block : public std::enable_shared_from_this<Block> {
public:
  Block(int start, int end) : start_(start), end_(end) {};
  int start() const;
  int end() const;
  bool IsInBlock(int stmtNo) const;
  std::vector<std::weak_ptr<Block>> parents() const;
  std::vector<std::weak_ptr<Block>> children() const;
  void AddParent(std::shared_ptr<Block> parent);
  void AddChild(std::shared_ptr<Block> child);

private:
  std::vector<std::weak_ptr<Block>> parents_;
  std::vector<std::weak_ptr<Block>> children_;
  int start_;
  int end_;

  void AddParentOnly(std::shared_ptr<Block> parent);
  void AddChildOnly(std::shared_ptr<Block> child);
};
}
