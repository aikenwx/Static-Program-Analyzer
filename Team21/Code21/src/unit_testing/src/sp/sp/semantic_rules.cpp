#include "catch.hpp"

#include "PKB/PKB.h"
#include "sp/sp.h"

TEST_CASE("SP should terminate if there are non-unique procedure names") {
  std::string program = R"(procedure main {
    x = 1;
    read y;
    print x;
  }
  procedure main {
    x = 1;
    read y;
    print x;
  })";

  PKB pkb = PKB();

  sp::SP sp = sp::SP();

  REQUIRE_THROWS_WITH(sp.process(program, &pkb), "Procedure names must be unique");
}
