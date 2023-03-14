#include <catch2/catch_test_macros.hpp>

#include "PKB/PKB.h"
#include "sp/sp.h"

namespace test_sp {
TEST_CASE("SP can process a simple program") {
  std::string program = R"(procedure main {
    x = 1;
    read y;
    print x;
})";

  PKB pkb = PKB();

  sp::SP sp = sp::SP();
  sp.process(program, &pkb);
}

void foo(std::string bar) {

}
}  // namespace test_sp
