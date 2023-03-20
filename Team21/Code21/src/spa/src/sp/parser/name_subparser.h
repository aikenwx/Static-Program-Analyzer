#pragma once

#include "subparser.h"

namespace parser {
class NameSubparser : public Subparser {
public:
  bool Parse(std::shared_ptr<Context> context) override;
};
}  // namespace parser
