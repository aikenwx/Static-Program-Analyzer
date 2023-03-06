#pragma once

#include "context.h"
#include "i_subparser.h"

namespace parser {
class Subparser : public ISubparser {
public:
  Subparser();

  std::shared_ptr<ISubparser> SetNext(std::shared_ptr<ISubparser> next) override;
  bool Parse(std::shared_ptr<Context> context) override;

private:
  std::shared_ptr<ISubparser> next;
};
}
