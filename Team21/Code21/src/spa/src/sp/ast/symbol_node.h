#pragma once
#include "i_node.h"

class SymbolNode : public INode {
public:
  SymbolNode(SymbolType type);

  SymbolType getType();

private:
  SymbolType type;
};

enum SymbolType {
  kAnd,
  kAssign,
  kDivide,
  kEqual,
  kGreater,
  kGreaterEqual,
  kLeftBrace,
  kLeftParen,
  kLesser,
  kLesserEqual,
  kMinus,
  kModulo,
  kMultiply,
  kNotEqual,
  kNot,
  kOr,
  kPlus,
  kRightBrace,
  kRightParen,
  kSemicolon
};