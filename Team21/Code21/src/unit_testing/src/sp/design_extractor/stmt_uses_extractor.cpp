#include "sp/design_extractor/stmt_uses_extractor.h"

#include <catch2/catch_test_macros.hpp>

#include "sp/ast/assign_node.h"
#include "sp/rel/uses_stmt_var_relationship.h"
#include "util/unique_ptr_cast.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
namespace test_design_extractor {
SCENARIO("Uses extractor should be able to handle single-level assign node "
         "appropriately") {
  GIVEN("Assign node") {
    auto assignNode = std::make_shared<ast::AssignNode>(
        std::make_shared<ast::IdentifierNode>("a"),
        std::make_shared<ast::ExpressionNode>(
            std::make_shared<ast::IdentifierNode>("b")));
    assignNode->SetStatementNumber(15);

    WHEN("Variable node from LHS of assign node is handled") {
      auto extractor = design_extractor::StmtUsesExtractor();

      extractor.HandleAssignNode(assignNode, 0);

      extractor.HandleVariableNode(assignNode->GetVariable(), 1);

      THEN("No uses relationship should be created") {
        REQUIRE(extractor.GetRelationships().empty());
      }
    };

    WHEN("Variable node from RHS of assign node is handled") {
      auto extractor = design_extractor::StmtUsesExtractor();

      // we can use AcceptVisitor here because LHS isn't gonna hit on that
      assignNode->AcceptVisitor(extractor, 0);

      std::vector<std::shared_ptr<rel::UsesStmtVarRelationship>> relns =
          extractor.GetRelationships();

      THEN("Uses relationship should be created") {
        REQUIRE(relns.size() == 1);

        std::shared_ptr<rel::UsesStmtVarRelationship> usesRel = relns[0];

        REQUIRE(usesRel->statementNumber() == assignNode->GetStatementNumber());

        // i mean, this has to be the case, right?
        std::shared_ptr<ast::IdentifierNode> varNode =
            std::static_pointer_cast<ast::IdentifierNode>(
                assignNode->GetAssignment());

        REQUIRE(usesRel->variableName() == varNode->GetValue());
      }
    }
  };
}
} // namespace test_design_extractor

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
