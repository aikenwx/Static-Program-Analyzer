#include "PKB/PKB.h"
#include "catch.hpp"
#include "exceptions/syntax_error.h"
#include "sp/sp.h"

SCENARIO("SP should terminate if there are syntax errors in the program", "[sp]") {
  PKB pkb = PKB();
  sp::SP sp = sp::SP();

  GIVEN("A program containing an invalid identifier") {
    std::string program = R"(procedure main {
      read a_e;
    })";

    WHEN("The program is processed") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid identifier"));
      }
    }
  };

  GIVEN("A program containing an unexpected symbol") {
    std::string program = R"(procedure main {
      e = a &$ f;
    })";

    WHEN("The program is processed") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SyntaxError,
            Catch::Message("Syntax error: Unexpected symbol"));
      }
    }
  };

  GIVEN("An empty program") {
    std::string program = "";

    WHEN("The program is processed") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SyntaxError,
            Catch::Message("Syntax error: Empty program"));
      }
    }
  };

  GIVEN("A program containing an empty procedure") {
    std::string program = R"(procedure main {})";

    WHEN("The program is processed") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid program"));
      }
    }
  };

  GIVEN("A program made up of non-procedure items") {
    std::string program = R"(1 + 3 == 4)";

    WHEN("The program is processed") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid program"));
      }
    }
  };
}
