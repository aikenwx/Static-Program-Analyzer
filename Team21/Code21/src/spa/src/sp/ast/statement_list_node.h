#pragma once

#include <memory>
#include <vector>

#include "i_node.h"

namespace ast {
class StatementListNode : public INode {
public:
  void AddStatement(std::shared_ptr<INode> node);
  // Format is a stack, i.e. statements are stored in reverse order
  std::vector<std::shared_ptr<INode>> GetStatements();
  std::string ToString() const override;

private:
  std::vector<std::shared_ptr<INode> > statements;
};
}
