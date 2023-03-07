#pragma once

#include <memory>

#include "statement_node.h"
#include "name_node.h"

namespace ast {
class CallNode : public StatementNode {
public:
  CallNode(std::shared_ptr<NameNode> name);

  std::string GetProcedureName();
  std::string ToString() const override;

  void AcceptVisitor(std::shared_ptr<INode> currentNode,
                     std::shared_ptr<design_extractor::Extractor> extractor,
                     int depth) override;

private:
  std::string name;
};
}
