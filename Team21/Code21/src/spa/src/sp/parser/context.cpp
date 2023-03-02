#include "context.h"
#include "util/instance_of.h"

namespace parser {
template<typename T>
bool Context::IsLookaheadTypeOf() {
  return util::instance_of<T>(*lookahead);
}

std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>> Context::GetStack() {
  return std::shared_ptr<std::vector<std::shared_ptr<ast::INode>>>(stack);
}
}
