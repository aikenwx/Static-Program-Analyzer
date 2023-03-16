#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "exceptions/semantic_error.h"
#include "PKB/PKB.h"
#include "sp/sp.h"

namespace test_sp {
using Catch::Matchers::Message;
using Catch::Matchers::MessageMatches;
using Catch::Matchers::StartsWith;

SCENARIO("SP should terminate if there are non-unique procedure names") {
  GIVEN("A program with duplicate procedure names") {
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

    WHEN("SP is run") {
      auto pkb = PKB();

      auto sp = sp::SP();

      THEN("SP should terminate with a semantic error") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SemanticError,
            MessageMatches(StartsWith("Semantic error: Duplicate procedure name: ")));
      }
    }
  }
}

SCENARIO("SP should terminate if a nonexistent procedure is called") {
  GIVEN("A program with a call to a nonexistent procedure") {
    std::string program = R"(procedure main {
      x = 1;
      read y;
      print x;
      call nonexistent;
    })";

    WHEN("SP is run") {
      auto pkb = PKB();

      auto sp = sp::SP();

      THEN("SP should terminate with a semantic error") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SemanticError,
            Message("Semantic error: Call node cannot call procedure "
                           "that does not exist"));
      }
    }
  }
}

SCENARIO("SP should terminate if a recursive procedure call exists") {
  GIVEN("A program with a procedure that calls itself") {
    std::string program = R"(procedure recursive {
      call recursive;
    })";

    WHEN("SP is run") {
      auto pkb = PKB();

      auto sp = sp::SP();

      THEN("SP should terminate with a semantic error") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SemanticError,
            MessageMatches(StartsWith("Semantic error: Procedure call is recursive: ")));
      }
    }
  }

  GIVEN(
      "A program with a procedure that calls another procedure that calls "
      "itself") {
    std::string program = R"(procedure recursive {
      call other;
    }
    procedure other {
      call recursive;
    })";

    WHEN("SP is run") {
      auto pkb = PKB();

      auto sp = sp::SP();

      THEN("SP should terminate with a semantic error") {
        REQUIRE_THROWS_MATCHES(
            sp.process(program, &pkb), exceptions::SemanticError,
            MessageMatches(StartsWith("Semantic error: Procedure call is recursive: ")));
      }
    }
  }
}
}  // namespace test_sp
