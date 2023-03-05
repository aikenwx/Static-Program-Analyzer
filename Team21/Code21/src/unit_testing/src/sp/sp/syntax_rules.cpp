#include "PKB/PKB.h"
#include "catch.hpp"
#include "exceptions/syntax_error.h"
#include "sp/sp.h"

TEST_CASE("SP should terminate there are unexpected characters in the program",
          "[sp]") {
  PKB pkb = PKB();
  sp::SP sp = sp::SP();

  std::string program = R"(procedure main {
    read a_e;
  })";

  REQUIRE_THROWS_MATCHES(sp.process(program, &pkb), exceptions::SyntaxError,
                         Catch::Message("Syntax error: Invalid identifier"));

  program = R"(procedure main {
    e = a &$ f;
  })";

  REQUIRE_THROWS_MATCHES(sp.process(program, &pkb), exceptions::SyntaxError,
                         Catch::Message("Syntax error: Unexpected symbol"));
};

TEST_CASE("SP should terminate if invalid programs are given", "[sp]") {
  PKB pkb = PKB();
  sp::SP sp = sp::SP();

  std::string program = R"(procedure main {})";
  REQUIRE_THROWS_MATCHES(sp.process(program, &pkb), exceptions::SyntaxError,
                         Catch::Message("Syntax error: Invalid program"));

  program = R"(1 + 3 == 4)";
  REQUIRE_THROWS_MATCHES(sp.process(program, &pkb), exceptions::SyntaxError,
                         Catch::Message("Syntax error: Invalid program"));

  program = "";
  REQUIRE_THROWS_MATCHES(sp.process(program, &pkb), exceptions::SyntaxError,
                         Catch::Message("Syntax error: Empty program"));
}
