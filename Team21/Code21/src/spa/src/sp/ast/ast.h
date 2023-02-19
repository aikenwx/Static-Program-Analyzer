#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class AST {
public:
  std::shared_ptr<INode> GetRoot();
  void SetRoot(std::shared_ptr<INode> node);

private:
  std::shared_ptr<INode> root;
};
}
