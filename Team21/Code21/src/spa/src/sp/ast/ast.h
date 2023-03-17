#pragma once

#include <memory>

#include "i_node.h"

namespace ast {
class AST {
public:
 auto GetRoot() -> std::shared_ptr<INode>;
 void SetRoot(std::shared_ptr<INode> node);

private:
  std::shared_ptr<INode> root;
};
}
