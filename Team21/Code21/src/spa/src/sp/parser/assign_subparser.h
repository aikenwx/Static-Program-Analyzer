#pragma once

#include "subparser.h"

namespace parser {
class AssignSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context) override;
};
}
