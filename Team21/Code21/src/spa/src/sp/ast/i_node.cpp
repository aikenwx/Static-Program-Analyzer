#include "i_node.h"

namespace ast {
std::ostream &operator<<(std::ostream &out, const INode &node) {
  node.Write(out);
  return out;
}
}
