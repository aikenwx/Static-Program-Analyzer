#pragma once

#include "subparser.h"

namespace parser {
class WhileSubparser : public Subparser {
  bool Parse(std::shared_ptr<Context> context) override;
};
}
