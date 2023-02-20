#include "sp/design_extractor/stmt_uses_extractor.h"

#include "catch.hpp"
#include "sp/ast/assign_node.h"
#include "sp/design_extractor/traverse.h"
#include "sp/rel/uses_stmt_var_relationship.h"
#include "util/unique_ptr_cast.h"

namespace test_design_extractor {
SCENARIO(
    "Uses extractor should be able to handle single-level assign node "
    "appropriately") {
  GIVEN("Assign node") {
    std::shared_ptr<ast::AssignNode> assignNode =
        std::make_shared<ast::AssignNode>(
            std::make_shared<ast::VariableNode>("a"),
            std::make_shared<ast::ExpressionNode>(
                std::make_shared<ast::VariableNode>("b")));
    assignNode->SetStatementNumber(15);

    WHEN("Variable node from LHS of assign node is handled") {
      std::vector<std::shared_ptr<ast::INode>> parents = {assignNode};
      design_extractor::StmtUsesExtractor extractor =
          design_extractor::StmtUsesExtractor();

      std::optional<std::vector<std::unique_ptr<rel::Relationship>>> relns =
          extractor.HandleVariableNode(parents, assignNode->GetVariable());
      THEN("No uses relationship should be created") {
        REQUIRE(relns.has_value());
        REQUIRE(relns.value().empty());
      }
    };

    WHEN("Variable node from RHS of assign node is handled") {
      // we can use Traverse here because LHS isn't gonna hit on that
      std::optional<std::vector<std::unique_ptr<rel::Relationship>>> relns =
          Traverse(assignNode,
                   std::make_shared<design_extractor::StmtUsesExtractor>());

      THEN("Uses relationship should be created") {
        REQUIRE(relns.has_value());
        REQUIRE(relns.value().size() == 1);

        REQUIRE(relns.value()[0]->relationshipType() ==
                rel::RelationshipType::USES_STMT_VAR);
        std::unique_ptr<rel::UsesStmtVarRelationship> usesRel =
            std::static_pointer_cast<rel::UsesStmtVarRelationship>(
                std::move(relns.value()[0]));

        REQUIRE(usesRel->statementNumber() == assignNode->GetStatementNumber());

        // i mean, this has to be the case, right?
        std::shared_ptr<ast::VariableNode> varNode =
            std::static_pointer_cast<ast::VariableNode>(
                assignNode->GetAssignment());

        REQUIRE(usesRel->variableName() == varNode->GetName());
      }
    }
  };
}
}  // namespace test_design_extractor
