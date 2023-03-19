#include "subparser.h"

namespace parser {
Subparser::Subparser() {
  this->next = nullptr;
}

auto Subparser::SetNext(std::shared_ptr<ISubparser> next)
    -> std::shared_ptr<ISubparser> {
  this->next = next;
  return next;
}

auto Subparser::Parse(std::shared_ptr<Context> context) -> bool {
  if (this->next) {
    return this->next->Parse(context);
  }
  return false;
}
}  // namespace parser
