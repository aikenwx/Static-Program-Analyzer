#pragma once

#include <iostream>
#include <string>

#include "../design_extractor/extractor.h"

namespace ast {
class INode {
 public:
  friend std::ostream &operator<<(std::ostream &out, const INode &node);

  virtual std::string ToString() const = 0;

  /*
    Method for accepting visitors
  */
  virtual void AcceptVisitor(
      std::shared_ptr<INode> currentNode,
      std::shared_ptr<design_extractor::Extractor> extractor, int depth) = 0;
};
}  // namespace ast
