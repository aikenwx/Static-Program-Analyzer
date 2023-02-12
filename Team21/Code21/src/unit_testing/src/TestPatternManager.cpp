#include "PKB/PatternManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "catch.hpp"

TEST_CASE("PatternManager Instantiates") {
    PatternManager *patternManager = new PatternManager();
    delete patternManager;
}

TEST_CASE("PatternManager can store assign statement postfix expression") {
    PatternManager *patternManager = new PatternManager();

    AssignStatement *assignStatement = new AssignStatement(1);
    std::string *postfixExpression = new std::string("ab-");

    patternManager->storeAssignStatementPostfixExpression(assignStatement, postfixExpression);

    REQUIRE(*assignStatement->getPostFixExpression() == "ab-");
  
    delete assignStatement;
    delete patternManager;
}
