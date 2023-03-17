#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKB/PatternManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"

TEST_CASE("PatternManager Instantiates") {
    PatternManager *patternManager = new PatternManager();
    delete patternManager;
}

TEST_CASE("PatternManager can store assign statement postfix expression") {
    PatternManager *patternManager = new PatternManager();

    AssignStatement *assignStatement = new AssignStatement(1);
    std::shared_ptr<std::string> postfixExpression = std::make_shared<std::string>("ab-");

    patternManager->storeAssignStatementPostfixExpression(assignStatement, postfixExpression);

    REQUIRE(*assignStatement->getPostFixExpression() == "ab-");

    delete assignStatement;
    delete patternManager;
}
