#pragma once

#include "subparser.h"

namespace parser {
class TermSubparser : public Subparser {
public:
  bool Parse(std::shared_ptr<Context> context) override;
};
}  // namespace parser
