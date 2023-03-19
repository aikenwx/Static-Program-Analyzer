#pragma once

#include "subparser.h"

namespace parser {
class FactorSubparser : public Subparser {
public:
  auto Parse(std::shared_ptr<Context> context) -> bool override;
};
}  // namespace parser
