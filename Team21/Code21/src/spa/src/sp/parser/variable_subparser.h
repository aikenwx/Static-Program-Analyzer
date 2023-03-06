#pragma once

#include "subparser.h"

namespace parser {
class VariableSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context);
};
}
