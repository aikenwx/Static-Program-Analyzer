#pragma once

#include <iostream>
#include <string>

#include "../design_extractor/extractor.h"

namespace ast {
class INode {
 public:
  INode() = default;
  virtual ~INode() = default;
  INode(const INode &) = delete;
  auto operator=(const INode &) -> INode & = delete;
  INode(INode &&) = delete;
  auto operator=(INode &&) -> INode & = delete;

  friend auto operator<<(std::ostream &out, const INode &node)
      -> std::ostream &;

  [[nodiscard]] virtual auto ToString() const -> std::string = 0;

  /*
    Method for accepting visitors
  */
  virtual void AcceptVisitor(
      std::shared_ptr<INode> currentNode,
      std::shared_ptr<design_extractor::Extractor> extractor, int depth){};
};
}  // namespace ast
