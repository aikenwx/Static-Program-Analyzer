#pragma once

#include "subparser.h"

namespace parser {
class StatementListSubparser : public Subparser {
public:
  bool Parse(std::shared_ptr<Context> context) override;
};
}  // namespace parser
