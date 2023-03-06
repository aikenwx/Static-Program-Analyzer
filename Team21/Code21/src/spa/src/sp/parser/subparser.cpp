#include "subparser.h"

namespace parser {
Subparser::Subparser() {
  this->next = nullptr;
}

std::shared_ptr<ISubparser> Subparser::SetNext(std::shared_ptr<ISubparser> next) {
  this->next = next;
  return next;
}

bool Subparser::Parse(std::shared_ptr<Context> context) {
  if (this->next) {
    return this->next->Parse(context);
  }
  return false;
}
}
