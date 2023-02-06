#pragma once
#include <iostream>

namespace ast {
class INode {
public:
  //virtual void Execute() = 0;

  /*
    Method for accepting visitors
  */
  //virtual void AcceptVisitor() = 0;

  friend std::ostream &operator<<(std::ostream &out, const INode &node);

  virtual std::ostream &Write(std::ostream &out) const = 0;
};
}
