#pragma once

class INode {
public:
  virtual void execute() = 0;

  /*
    Method for accepting visitors
  */
  //virtual void acceptVisitor() = 0;
};