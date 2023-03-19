#pragma once

#include "context.h"
#include "i_subparser.h"

namespace parser {
class Subparser : public ISubparser {
public:
  Subparser();

  auto SetNext(std::shared_ptr<ISubparser> next) -> std::shared_ptr<ISubparser> override;
  auto Parse(std::shared_ptr<Context> context) -> bool override;

private:
  std::shared_ptr<ISubparser> next;
};
}  // namespace parser
