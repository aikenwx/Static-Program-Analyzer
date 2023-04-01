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

  auto pkb = PKB();

  sp::SP::Process(program, &pkb);
}
}  // namespace test_sp
