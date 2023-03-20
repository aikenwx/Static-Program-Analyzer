#pragma once

#include <iostream>
#include <string>

#include "../design_extractor/extractor.h"

namespace ast {
class INode : public std::enable_shared_from_this<INode> {
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
  virtual void AcceptVisitor(design_extractor::Extractor &extractor, int depth){
      // default no-op implementation
  };
};
} // namespace ast
