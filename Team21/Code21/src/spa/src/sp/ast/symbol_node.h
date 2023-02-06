#pragma once
#include "i_node.h"

namespace ast {
enum class SymbolType {
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

class SymbolNode : public INode {
public:
  SymbolNode(SymbolType type);

  SymbolType getType();

private:
  SymbolType type;
};
}
