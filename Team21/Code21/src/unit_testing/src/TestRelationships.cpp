
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <stdexcept>

#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBtestHelpers.h"

TEST_CASE("Relationships can instantiate") {
  auto printStatementOwner = std::make_shared<PrintStatement>(TWO);
  auto readStatementOwner = std::make_shared<ReadStatement>(THREE);
  auto whileStatementOwner = std::make_shared<WhileStatement>(FOUR);
  auto assignStatementOwner = std::make_shared<AssignStatement>(FIVE);
  auto procedureOwner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto procedure2Owner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *printStatement = printStatementOwner.get();
  auto *readStatement = readStatementOwner.get();
  auto *whileStatement = whileStatementOwner.get();
  auto *assignStatement = assignStatementOwner.get();
  auto *procedure = procedureOwner.get();
  auto *variable = variableOwner.get();
  auto *procedure2 = procedure2Owner.get();
  auto *variable2 = variable2Owner.get();

  auto callsRelationship =
      std::make_shared<CallsRelationship>(procedure, procedure2);
  auto callsStarRelationship =
      std::make_shared<CallsStarRelationship>(procedure, procedure2);
  auto parentRelationship =
      std::make_shared<ParentRelationship>(whileStatement, assignStatement);
  auto usesRelationship =
      std::make_shared<UsesRelationship>(procedure, variable);
  auto modifiesRelationship =
      std::make_shared<ModifiesRelationship>(procedure2, variable2);
  auto followsRelationship =
      std::make_shared<FollowsRelationship>(printStatement, readStatement);
  auto parentStarRelationship =
      std::make_shared<ParentStarRelationship>(whileStatement, assignStatement);

  auto followsStarRelationship =
      std::make_shared<FollowsStarRelationship>(printStatement, readStatement);
}

TEST_CASE("Relationships contain the correct left entity") {
  auto printStatementOwner = std::make_shared<PrintStatement>(TWO);
  auto readStatementOwner = std::make_shared<ReadStatement>(THREE);
  auto whileStatementOwner = std::make_shared<WhileStatement>(FOUR);
  auto assignStatementOwner = std::make_shared<AssignStatement>(FIVE);
  auto procedureOwner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto procedure2Owner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *printStatement = printStatementOwner.get();
  auto *readStatement = readStatementOwner.get();
  auto *whileStatement = whileStatementOwner.get();
  auto *assignStatement = assignStatementOwner.get();
  auto *procedure = procedureOwner.get();
  auto *variable = variableOwner.get();
  auto *procedure2 = procedure2Owner.get();
  auto *variable2 = variable2Owner.get();

  auto callsRelationship =
      std::make_shared<CallsRelationship>(procedure, procedure2);
  auto callsStarRelationship =
      std::make_shared<CallsStarRelationship>(procedure, procedure2);
  auto parentRelationship =
      std::make_shared<ParentRelationship>(whileStatement, assignStatement);
  auto usesRelationship =
      std::make_shared<UsesRelationship>(procedure, variable);
  auto modifiesRelationship =
      std::make_shared<ModifiesRelationship>(procedure2, variable2);
  auto followsRelationship =
      std::make_shared<FollowsRelationship>(printStatement, readStatement);
  auto parentStarRelationship =
      std::make_shared<ParentStarRelationship>(whileStatement, assignStatement);

  auto followsStarRelationship =
      std::make_shared<FollowsStarRelationship>(printStatement, readStatement);

  REQUIRE(parentRelationship->containsEntityOnLeftHand(whileStatement));
  REQUIRE(usesRelationship->containsEntityOnLeftHand(procedure));
  REQUIRE(modifiesRelationship->containsEntityOnLeftHand(procedure2));
  REQUIRE(followsRelationship->containsEntityOnLeftHand(printStatement));
  REQUIRE(parentStarRelationship->containsEntityOnLeftHand(whileStatement));
  REQUIRE(followsStarRelationship->containsEntityOnLeftHand(printStatement));
  REQUIRE(callsRelationship->containsEntityOnLeftHand(procedure));
  REQUIRE(callsStarRelationship->containsEntityOnLeftHand(procedure));
}

TEST_CASE("Relationships contain the correct right entity") {
  auto printStatementOwner = std::make_shared<PrintStatement>(TWO);
  auto readStatementOwner = std::make_shared<ReadStatement>(THREE);
  auto whileStatementOwner = std::make_shared<WhileStatement>(FOUR);
  auto assignStatementOwner = std::make_shared<AssignStatement>(FIVE);
  auto procedureOwner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto procedure2Owner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *printStatement = printStatementOwner.get();
  auto *readStatement = readStatementOwner.get();
  auto *whileStatement = whileStatementOwner.get();
  auto *assignStatement = assignStatementOwner.get();
  auto *procedure = procedureOwner.get();
  auto *variable = variableOwner.get();
  auto *procedure2 = procedure2Owner.get();
  auto *variable2 = variable2Owner.get();

  auto callsRelationship =
      std::make_shared<CallsRelationship>(procedure, procedure2);
  auto callsStarRelationship =
      std::make_shared<CallsStarRelationship>(procedure, procedure2);
  auto parentRelationship =
      std::make_shared<ParentRelationship>(whileStatement, assignStatement);
  auto usesRelationship =
      std::make_shared<UsesRelationship>(procedure, variable);
  auto modifiesRelationship =
      std::make_shared<ModifiesRelationship>(procedure2, variable2);
  auto followsRelationship =
      std::make_shared<FollowsRelationship>(printStatement, readStatement);
  auto parentStarRelationship =
      std::make_shared<ParentStarRelationship>(whileStatement, assignStatement);

  auto followsStarRelationship =
      std::make_shared<FollowsStarRelationship>(printStatement, readStatement);

  REQUIRE(parentRelationship->containsEntityOnRightHand(assignStatement));
  REQUIRE(usesRelationship->containsEntityOnRightHand(variable));
  REQUIRE(modifiesRelationship->containsEntityOnRightHand(variable2));
  REQUIRE(followsRelationship->containsEntityOnRightHand(readStatement));
  REQUIRE(parentStarRelationship->containsEntityOnRightHand(assignStatement));
  REQUIRE(followsStarRelationship->containsEntityOnRightHand(readStatement));
  REQUIRE(callsRelationship->containsEntityOnRightHand(procedure2));
  REQUIRE(callsStarRelationship->containsEntityOnRightHand(procedure2));
}

TEST_CASE("Relationships contain the wrong left entity") {
  auto printStatementOwner = std::make_shared<PrintStatement>(TWO);
  auto readStatementOwner = std::make_shared<ReadStatement>(THREE);
  auto whileStatementOwner = std::make_shared<WhileStatement>(FOUR);
  auto assignStatementOwner = std::make_shared<AssignStatement>(FIVE);
  auto procedureOwner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto procedure2Owner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *printStatement = printStatementOwner.get();
  auto *readStatement = readStatementOwner.get();
  auto *whileStatement = whileStatementOwner.get();
  auto *assignStatement = assignStatementOwner.get();
  auto *procedure = procedureOwner.get();
  auto *variable = variableOwner.get();
  auto *procedure2 = procedure2Owner.get();
  auto *variable2 = variable2Owner.get();

  auto callsRelationship =
      std::make_shared<CallsRelationship>(procedure, procedure2);
  auto callsStarRelationship =
      std::make_shared<CallsStarRelationship>(procedure, procedure2);
  auto parentRelationship =
      std::make_shared<ParentRelationship>(whileStatement, assignStatement);
  auto usesRelationship =
      std::make_shared<UsesRelationship>(procedure, variable);
  auto modifiesRelationship =
      std::make_shared<ModifiesRelationship>(procedure2, variable2);
  auto followsRelationship =
      std::make_shared<FollowsRelationship>(printStatement, readStatement);
  auto parentStarRelationship =
      std::make_shared<ParentStarRelationship>(whileStatement, assignStatement);

  auto followsStarRelationship =
      std::make_shared<FollowsStarRelationship>(printStatement, readStatement);

  REQUIRE_FALSE(parentRelationship->containsEntityOnLeftHand(assignStatement));
  REQUIRE_FALSE(usesRelationship->containsEntityOnLeftHand(variable));
  REQUIRE_FALSE(modifiesRelationship->containsEntityOnLeftHand(variable2));
  REQUIRE_FALSE(followsRelationship->containsEntityOnLeftHand(readStatement));
  REQUIRE_FALSE(
      parentStarRelationship->containsEntityOnLeftHand(assignStatement));
  REQUIRE_FALSE(
      followsStarRelationship->containsEntityOnLeftHand(readStatement));
  REQUIRE_FALSE(callsRelationship->containsEntityOnLeftHand(procedure2));
  REQUIRE_FALSE(callsStarRelationship->containsEntityOnLeftHand(procedure2));
}

TEST_CASE("Relationships contain the wrong right entity") {
  auto printStatementOwner = std::make_shared<PrintStatement>(TWO);
  auto readStatementOwner = std::make_shared<ReadStatement>(THREE);
  auto whileStatementOwner = std::make_shared<WhileStatement>(FOUR);
  auto assignStatementOwner = std::make_shared<AssignStatement>(FIVE);
  auto procedureOwner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto procedure2Owner =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *printStatement = printStatementOwner.get();
  auto *readStatement = readStatementOwner.get();
  auto *whileStatement = whileStatementOwner.get();
  auto *assignStatement = assignStatementOwner.get();
  auto *procedure = procedureOwner.get();
  auto *variable = variableOwner.get();
  auto *procedure2 = procedure2Owner.get();
  auto *variable2 = variable2Owner.get();

  auto callsRelationship =
      std::make_shared<CallsRelationship>(procedure, procedure2);
  auto callsStarRelationship =
      std::make_shared<CallsStarRelationship>(procedure, procedure2);
  auto parentRelationship =
      std::make_shared<ParentRelationship>(whileStatement, assignStatement);
  auto usesRelationship =
      std::make_shared<UsesRelationship>(procedure, variable);
  auto modifiesRelationship =
      std::make_shared<ModifiesRelationship>(procedure2, variable2);
  auto followsRelationship =
      std::make_shared<FollowsRelationship>(printStatement, readStatement);
  auto parentStarRelationship =
      std::make_shared<ParentStarRelationship>(whileStatement, assignStatement);

  auto followsStarRelationship =
      std::make_shared<FollowsStarRelationship>(printStatement, readStatement);

  REQUIRE_FALSE(parentRelationship->containsEntityOnRightHand(whileStatement));
  REQUIRE_FALSE(usesRelationship->containsEntityOnRightHand(variable2));
  REQUIRE_FALSE(modifiesRelationship->containsEntityOnRightHand(variable));
  REQUIRE_FALSE(followsRelationship->containsEntityOnRightHand(printStatement));
  REQUIRE_FALSE(
      parentStarRelationship->containsEntityOnRightHand(whileStatement));
  REQUIRE_FALSE(
      followsStarRelationship->containsEntityOnRightHand(printStatement));
  REQUIRE_FALSE(callsRelationship->containsEntityOnRightHand(procedure));
  REQUIRE_FALSE(callsStarRelationship->containsEntityOnRightHand(procedure));
}
TEST_CASE(
    "Instantiating a ModifiesRelationship with a non-Procedure or "
    "non-Statement as LHS throws an exception") {
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *variable = variableOwner.get();
  auto *variable2 = variable2Owner.get();

  REQUIRE_THROWS_AS(std::make_shared<ModifiesRelationship>(variable2, variable),
                    std::invalid_argument);
}

TEST_CASE(
    "Instantiating a UsesRelationship with a non-Procedure or non-Statement as "
    "LHS entity throws an exception") {
  auto variableOwner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto variable2Owner =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto *variable = variableOwner.get();
  auto *variable2 = variable2Owner.get();

  REQUIRE_THROWS_AS(std::make_shared<UsesRelationship>(variable2, variable),
                    std::invalid_argument);
}
