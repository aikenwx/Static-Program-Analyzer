#pragma once

#include "subparser.h"

namespace parser {
class RelationalFactorSubparser : public Subparser {
public:
  auto Parse(std::shared_ptr<Context> context) -> bool override;

private:
  static auto ParsePrimaryCase(const std::shared_ptr<Context> &context) -> bool;
  static auto ParseSecondaryCase(const std::shared_ptr<Context> &context) -> bool;
};
}  // namespace parser
