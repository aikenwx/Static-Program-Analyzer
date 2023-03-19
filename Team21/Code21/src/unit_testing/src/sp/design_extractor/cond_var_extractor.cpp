#include "sp/design_extractor/cond_var_extractor.h"

#include <catch2/catch_test_macros.hpp>
#include <unordered_set>

#include "sp/ast/if_node.h"
#include "sp/parser/simple_chain_parser.h"
#include "sp/tokenizer/simple_tokenizer.h"

namespace test_design_extractor {
template <typename T>
auto ExtractCondVars(std::string_view source)
    -> std::unordered_map<int, std::unordered_set<std::string>> {
  auto tokenizer = tokenizer::SimpleTokenizer();
  auto tokens = tokenizer.tokenize(source);

  auto parser = parser::SimpleChainParser();
  auto ast = parser.Parse(std::move(tokens));
  auto node = ast->GetRoot();

  auto extractor = design_extractor::CondVarExtractor<T>();
  node->AcceptVisitor(extractor, 0);

  return extractor.condVars();
}

SCENARIO("CondVarExtractor extracts nothing from conditional nodes with no "
         "variables") {
  GIVEN("An if node with no variables") {
    std::string source = "if (1 == 2) then { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::IfNode>(source);

      for (const auto &[key, value] : varMap) {
        std::cout << key << ": ";
        for (const auto &var : value) {
          std::cout << var << " ";
        }
        std::cout << std::endl;
      }

      THEN("The variables should be empty") { REQUIRE(varMap.empty()); }

      THEN("The variables should be empty") { REQUIRE(varMap.empty()); }
    }
  }

  GIVEN("A while node with no variables") {
    std::string source = "while (1 == 2) { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::WhileNode>(source);

      THEN("The variables should be empty") { REQUIRE(varMap.empty()); }
    }
  }
}

SCENARIO("CondVarExtractor can extract variables from conditional nodes with "
         "one variable") {
  GIVEN("An if node with a single variable") {
    std::string source = "if (a == 3) then { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::IfNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 1);
        REQUIRE(vars.find("a") != vars.end());
      }
    }
  }

  GIVEN("A while node with a single variable") {
    std::string source = "while (a == 3) { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::WhileNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 1);
        REQUIRE(vars.find("a") != vars.end());
      }
    }
  }
}

SCENARIO("CondVarExtractor can extract variables from conditional nodes with "
         "multiple variables") {
  GIVEN("An if node with two variables") {
    std::string source = "if (a == b) then { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::IfNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 2);
        REQUIRE(vars.find("a") != vars.end());
        REQUIRE(vars.find("b") != vars.end());
      }
    }
  }

  GIVEN("A while node with two variables") {
    std::string source = "while (a == b) { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::WhileNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 2);
        REQUIRE(vars.find("a") != vars.end());
        REQUIRE(vars.find("b") != vars.end());
      }
    }
  }

  GIVEN("A conditional node with two of the same variable") {
    std::string source = "if (a == a) then { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::IfNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 1);
        REQUIRE(vars.find("a") != vars.end());
      }
    }
  }

  GIVEN("An if node with more than two variables") {
    std::string source = "if ((a == b) && (c == 2)) then { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::IfNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 3);
        REQUIRE(vars.find("a") != vars.end());
        REQUIRE(vars.find("b") != vars.end());
        REQUIRE(vars.find("c") != vars.end());
      }
    }
  }

  GIVEN("A while node with two variables and two constants") {
    std::string source = "while ((a == b) && (c == d)) { read x; } else { read y; }";

    WHEN("Extracting variables") {
      auto varMap = ExtractCondVars<ast::WhileNode>(source);

      THEN("The variables should be correct") {
        REQUIRE(varMap.size() == 1);
        auto vars = varMap.at(1);
        REQUIRE(vars.size() == 4);
        REQUIRE(vars.find("a") != vars.end());
        REQUIRE(vars.find("b") != vars.end());
        REQUIRE(vars.find("c") != vars.end());
        REQUIRE(vars.find("d") != vars.end());
      }
    }
  }
}
} // namespace test_design_extractor
