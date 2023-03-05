#pragma once

#include "subparser.h"

namespace parser {
class WhileSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context) override;
};
}
