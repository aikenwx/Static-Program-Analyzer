#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKB/PatternManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"

TEST_CASE("PatternManager Instantiates") {
    auto patternManager = std::make_shared<PatternManager>();
}

TEST_CASE("PatternManager can store assign statement postfix expression") {
    auto patternManager = std::make_shared<PatternManager>();

    auto assignStatement = std::make_shared<AssignStatement>(1);

    std::shared_ptr<std::string> postfixExpression = std::make_shared<std::string>("ab-");

    patternManager->storeAssignStatementPostfixExpression(assignStatement.get(), postfixExpression);

    REQUIRE(*assignStatement->getPostFixExpression() == "ab-");
}
