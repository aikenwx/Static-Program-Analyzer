#pragma once

#include "subparser.h"

namespace parser {
class RelationalFactorSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context);
};
}
