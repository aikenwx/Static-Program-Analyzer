#include "PKB/PKB.h"
#include "catch.hpp"
#include "sp/sp.h"

using namespace std;
void require(bool b) { REQUIRE(b); }

TEST_CASE("SP can process and store a simple program into PKB") {
  std::string program = R"(procedure main {
    x = 1;
    read y;
    print x;
})";

  PKB pkb = PKB();

  sp::SP sp = sp::SP();
  sp.process(program, &pkb);
}
