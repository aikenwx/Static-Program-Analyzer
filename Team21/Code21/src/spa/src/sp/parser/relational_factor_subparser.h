#pragma once

#include "subparser.h"

namespace parser {
class RelationalFactorSubparser : public Subparser {
public:
  auto Parse(std::shared_ptr<Context> context) -> bool override;

private:
  auto static ParsePrimaryCase(const std::shared_ptr<Context> &context) -> bool;
  auto static ParseSecondaryCase(const std::shared_ptr<Context> &context) -> bool;
};
}  // namespace parser
