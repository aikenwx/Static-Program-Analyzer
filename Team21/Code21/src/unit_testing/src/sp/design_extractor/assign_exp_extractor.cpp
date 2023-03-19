#include "sp/design_extractor/assign_exp_extractor.h"

#include <catch2/catch_test_macros.hpp>

#include "sp/parser/simple_chain_parser.h"
#include "sp/rel/assign_exp_relationship.h"
#include "sp/tokenizer/simple_tokenizer.h"
#include "sp/tokenizer/tokenizer.h"

namespace test_design_extractor {
auto ExtractAssignExp(
    std::string_view source) -> std::vector<std::shared_ptr<rel::AssignExpRelationship>> {
  auto tokenizer = tokenizer::SimpleTokenizer();
  auto tokens = tokenizer.tokenize(source);

  auto parser = parser::SimpleChainParser();
  auto ast = parser.Parse(std::move(tokens));
  auto node = ast->GetRoot();

  auto extractor = design_extractor::AssignExpExtractor();
  node->AcceptVisitor(extractor, 0);

  return extractor.GetRelationships();
}

SCENARIO("Assign exp extractor can extract single-element assign exp") {
  GIVEN("A single-const assign exp") {
    std::string source = "a = 1;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("1")");
      }
    }
  }

  GIVEN("A single-var assign exp") {
    std::string source = "a = b;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b")");
      }
    }
  }
}

SCENARIO("Assign exp extractor can extract single-operation assign exp") {
  GIVEN("A single-operation assign exp with an addition") {
    std::string source = "a = b + c;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"+)");
      }
    }
  }

  GIVEN("A single-operation assign exp with a subtraction") {
    std::string source = "a = b - c;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"-)");
      }
    }
  }

  GIVEN("A single-operation assign exp with a multiplication") {
    std::string source = "a = b * c;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"*)");
      }
    }
  }

  GIVEN("A single-operation assign exp with a division") {
    std::string source = "a = b / c;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"/)");
      }
    }
  }

  GIVEN("A single-operation assign exp with a modulo operation") {
    std::string source = "a = b % c;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"%)");
      }
    }
  }
}

SCENARIO("Assign exp extractor can extract multi-operation assign exp") {
  GIVEN("A single expression with multiple additions and subtractions") {
    std::string source = "a = b + c - d + e;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"+"d"-"e"+)");
      }
    }
  }

  GIVEN("A single expression with multiple multiplications and divisions") {
    std::string source = "a = b * c / d * e;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("b""c"*"d"/"e"*)");
      }
    }
  }

  GIVEN("A single expression with an addition of two multiplications") {
    std::string source = "a = a * b + c * d;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("a""b"*"c""d"*+)");
      }
    }
  }

  GIVEN("A single expression with a multiplication of two additions") {
    std::string source = "a = a + b * c + d;";

    WHEN("Extracting assign exp") {
      auto relns = ExtractAssignExp(source);

      THEN("The assign exp should be correct") {
        REQUIRE(relns.size() == 1);
        auto rel = relns.front();
        REQUIRE(rel->varName() == "a");
        REQUIRE(rel->postfixExp() == R"("a""b""c"*+"d"+)");
      }
    }
  }
}
} // namespace test_design_extractor
