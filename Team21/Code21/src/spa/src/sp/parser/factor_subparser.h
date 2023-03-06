#pragma once

#include "subparser.h"

namespace parser {
class FactorSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context);
};
}
