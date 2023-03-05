#pragma once

#include "subparser.h"

namespace parser {
class IfSubparser : public Subparser {
  bool Subparser::Parse(std::shared_ptr<Context> context) override;
};
}
