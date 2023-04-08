#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKB/PatternManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"

TEST_CASE("PatternManager Instantiates") {
    auto patternManager = std::make_unique<PatternManager>();
}

TEST_CASE("PatternManager can store assign statement postfix expression") {
    auto patternManager = std::make_unique<PatternManager>();

    auto assignStatement = std::make_unique<AssignStatement>(1);

    auto postfixExpression = std::make_unique<std::string>("ab-");

    patternManager->storeAssignStatementPostfixExpression(assignStatement.get(), std::move(postfixExpression));

    REQUIRE(*assignStatement->getPostFixExpression() == "ab-");
}
