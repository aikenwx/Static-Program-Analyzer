#pragma once

#include "context.h"

namespace parser {
class Subparser {
public:
  Subparser();

  std::shared_ptr<Subparser> SetNext(std::shared_ptr<Subparser> next);
  virtual bool Parse(std::shared_ptr<Context> context) = 0;

private:
  std::shared_ptr<Subparser> next;
};
}
