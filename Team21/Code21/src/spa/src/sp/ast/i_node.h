#pragma once
#include <iostream>
#include <string>

namespace ast {
class INode {
public:
  /*
    Method for accepting visitors
  */
  //virtual void AcceptVisitor() = 0;

  friend std::ostream &operator<<(std::ostream &out, const INode &node);

  virtual std::string ToString() const = 0;
};
}
