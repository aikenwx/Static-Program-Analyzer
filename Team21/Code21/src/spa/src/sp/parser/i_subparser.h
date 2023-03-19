#pragma once

#include "context.h"

namespace parser {
class ISubparser {
public:
  ISubparser() = default;
  ISubparser(const ISubparser &) = delete;
  ISubparser(ISubparser &&) = delete;
  auto operator=(const ISubparser &) -> ISubparser & = delete;
  auto operator=(ISubparser &&) -> ISubparser & = delete;
  virtual ~ISubparser() = default;
  virtual auto SetNext(std::shared_ptr<ISubparser> next) -> std::shared_ptr<ISubparser> = 0;
  virtual auto Parse(std::shared_ptr<Context> context) -> bool = 0;
};
}  //namespace parser
