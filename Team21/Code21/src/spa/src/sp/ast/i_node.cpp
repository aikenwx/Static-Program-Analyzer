#include "i_node.h"

namespace ast {
auto operator<<(std::ostream &out, const INode &node) -> std::ostream & {
  out << node.ToString();
  return out;
}
}
