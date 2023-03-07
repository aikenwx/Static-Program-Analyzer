#pragma once

#include "context.h"

namespace parser {
class ISubparser {
public:
  virtual std::shared_ptr<ISubparser> SetNext(std::shared_ptr<ISubparser> next) = 0;
  virtual bool Parse(std::shared_ptr<Context> context) = 0;
};
}
