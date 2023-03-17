#include <catch2/catch_test_macros.hpp>

#include "PKB/PopulateFacade.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

TEST_CASE("PopulateFacade can instantiate") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
}

TEST_CASE(
    "PopulateFacade can add postfix expressions to assignment statements") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeAssignmentStatement(1);
  populateFacade->storeAssignStatementPostfixExpression(1, "xy+");
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "1");
  auto key = EntityKey(&Statement::getEntityTypeStatic(), 1);
  entityManager->getEntity(key);
  REQUIRE(*((AssignStatement *)entityManager->getEntity(key))
               ->getPostFixExpression() == "xy+");
}

TEST_CASE(
    "Error thrown when trying to add non pattern expression to call "
    "statement") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeCallStatement(1);
  REQUIRE_THROWS(
      populateFacade->storeAssignStatementPostfixExpression(1, "xy+"));
}

TEST_CASE("PopulateFace can populate assign statement") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  std::string postfixExpression = "xy+";
  populateFacade->storeAssignmentStatement(1);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "1");
}

TEST_CASE("PopulateFace can populate call statement") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeCallStatement(2);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(CallStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() ==
          CallStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "2");
}

TEST_CASE("PopulateFace can populate while statement") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeWhileStatement(3);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(WhileStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() ==
          WhileStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "3");
}

TEST_CASE("PopulateFace can populate if statement") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeIfStatement(4);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(IfStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() ==
          IfStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "4");
}

TEST_CASE("PopulateFace can populate procedure") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string procedureName = "main";
  populateFacade->storeProcedure(procedureName);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(Procedure::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() == Procedure::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "main");
}

TEST_CASE("PopulateFace can populate variable") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  std::string variableName = "x";
  populateFacade->storeVariable(variableName);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(Variable::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() == Variable::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "x");
}

TEST_CASE("PopulateFace can populate constant") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  populateFacade->storeConstant(5);
  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(Constant::getEntityTypeStatic());
  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->getEntityType() == Constant::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "5");
}

TEST_CASE("PopulateFacade can populate follow relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeCallStatement(1);
  populateFacade->storePrintStatement(2);
  populateFacade->storeFollowsRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          FollowsRelationship::getRelationshipTypeStatic(),
          CallStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          FollowsRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE("PopulateFacade can populate parent relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeWhileStatement(1);
  populateFacade->storePrintStatement(2);
  populateFacade->storeParentRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          ParentRelationship::getRelationshipTypeStatic(),
          WhileStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          ParentRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE("PopulateFacade can populate parentStar relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeWhileStatement(1);
  populateFacade->storePrintStatement(2);
  populateFacade->storeParentStarRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          ParentStarRelationship::getRelationshipTypeStatic(),
          WhileStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          ParentStarRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE("PopulateFacade can populate followStar relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeCallStatement(1);
  populateFacade->storePrintStatement(2);
  populateFacade->storeFollowsStarRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          FollowsStarRelationship::getRelationshipTypeStatic(),
          CallStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          FollowsStarRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE(
    "Adding a relationship with a non-existent entity throws an exception") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  REQUIRE_THROWS_AS(populateFacade->storeParentRelationship(1, 2),
                    std::exception);
}

TEST_CASE("PopulateFacade can populate statement modifies relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string variableName = "variable";

  populateFacade->storeAssignmentStatement(1);
  populateFacade->storeVariable(variableName);
  populateFacade->storeStatementModifiesVariableRelationship(1, variableName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          ModifiesRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          Variable::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          ModifiesRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "variable");
}

TEST_CASE("PopulateFacade can populate statement uses relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  std::string variableName = "variable";

  populateFacade->storeAssignmentStatement(1);
  populateFacade->storeVariable(variableName);

  populateFacade->storeStatementUsesVariableRelationship(1, variableName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          UsesRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          Variable::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          UsesRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "variable");
}

TEST_CASE("PopulateFacade can populate statement ParentStar relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeAssignmentStatement(1);
  populateFacade->storePrintStatement(2);
  populateFacade->storeParentStarRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          ParentStarRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          ParentStarRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE("PopulateFacade can populate FollowsStar relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  populateFacade->storeAssignmentStatement(1);
  populateFacade->storePrintStatement(2);

  populateFacade->storeFollowsStarRelationship(1, 2);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          FollowsStarRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          PrintStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          FollowsStarRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");
}

TEST_CASE("PopulateFacade can populate procedure modifies relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string variableName = "variable";
  std::string procedureName = "Procedure";

  populateFacade->storeProcedure(procedureName);
  populateFacade->storeVariable(variableName);
  populateFacade->storeProcedureModifiesVariableRelationship(procedureName,
                                                             variableName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          ModifiesRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          ModifiesRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() ==
          "Procedure");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "variable");
}

TEST_CASE("PopulateFacade can populate procedure uses relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string variableName = "variable";
  std::string procedureName = "Procedure";

  populateFacade->storeVariable(variableName);
  populateFacade->storeProcedure(procedureName);
  populateFacade->storeProcedureUsesVariableRelationship(procedureName,
                                                         variableName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          UsesRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          UsesRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() ==
          "Procedure");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "variable");
}

TEST_CASE("PopulateFacade can populate Caller Calls Callee relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string callerName = "caller";
  std::string calleeName = "callee";

  populateFacade->storeProcedure(callerName);
  populateFacade->storeProcedure(calleeName);

  populateFacade->storeCallsRelationship(callerName, calleeName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          CallsRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          CallsRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() ==
          "caller");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "callee");
}

TEST_CASE("PopulateFacade can populate Caller Calls* Callee relationship") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  std::string callerName = "caller";
  std::string calleeName = "callee";

  populateFacade->storeProcedure(callerName);
  populateFacade->storeProcedure(calleeName);

  populateFacade->storeCallsStarRelationship(callerName, calleeName);

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          CallsStarRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          CallsStarRelationship::getRelationshipTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() ==
          "caller");
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() ==
          "callee");
}

TEST_CASE("PopulateFacade can populate multiple entities and relationships") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager = std::make_shared<RelationshipManager>();
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGManager>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  std::string postFixPattern = "xy+";

  populateFacade->storeAssignmentStatement(1);
  populateFacade->storeCallStatement(2);
  populateFacade->storeAssignmentStatement(3);
  populateFacade->storeAssignmentStatement(4);

  populateFacade->storeProcedure("Procedure");
  populateFacade->storeProcedure("Procedure2");

  populateFacade->storeParentStarRelationship(1, 2);
  populateFacade->storeFollowsStarRelationship(1, 3);
  populateFacade->storeFollowsRelationship(1, 3);
  populateFacade->storeFollowsRelationship(2, 4);
  populateFacade->storeCallsRelationship("Procedure", "Procedure2");
  populateFacade->storeCallsStarRelationship("Procedure", "Procedure2");

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());
  REQUIRE(entities->size() == 3);
  REQUIRE(entities->at(0)->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*entities->at(0)->getEntityValue() == "1");

  std::vector<Entity *> *entities2 =
      entityManager->getEntitiesByType(CallStatement::getEntityTypeStatic());
  REQUIRE(entities2->size() == 1);
  REQUIRE(entities2->at(0)->getEntityType() ==
          CallStatement::getEntityTypeStatic());
  REQUIRE(*entities2->at(0)->getEntityValue() == "2");

  std::vector<Relationship *> *relationships =
      relationshipManager->getRelationshipsByTypes(
          FollowsRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          AssignStatement::getEntityTypeStatic());
  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->getRelationshipType() ==
          FollowsRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "3");

  std::vector<Relationship *> *relationships2 =
      relationshipManager->getRelationshipsByTypes(
          FollowsRelationship::getRelationshipTypeStatic(),
          CallStatement::getEntityTypeStatic(),
          AssignStatement::getEntityTypeStatic());
  REQUIRE(relationships2->size() == 1);
  REQUIRE(relationships2->at(0)->getRelationshipType() ==
          FollowsRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships2->at(0)->getLeftHandEntity()->getEntityType() ==
          CallStatement::getEntityTypeStatic());
  REQUIRE(*relationships2->at(0)->getLeftHandEntity()->getEntityValue() == "2");
  REQUIRE(relationships2->at(0)->getRightHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships2->at(0)->getRightHandEntity()->getEntityValue() ==
          "4");

  std::vector<Relationship *> *relationships3 =
      relationshipManager->getRelationshipsByTypes(
          ParentStarRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          CallStatement::getEntityTypeStatic());
  REQUIRE(relationships3->size() == 1);
  REQUIRE(relationships3->at(0)->getRelationshipType() ==
          ParentStarRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships3->at(0)->getLeftHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships3->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(relationships3->at(0)->getRightHandEntity()->getEntityType() ==
          CallStatement::getEntityTypeStatic());
  REQUIRE(*relationships3->at(0)->getRightHandEntity()->getEntityValue() ==
          "2");

  std::vector<Relationship *> *relationships4 =
      relationshipManager->getRelationshipsByTypes(
          FollowsStarRelationship::getRelationshipTypeStatic(),
          AssignStatement::getEntityTypeStatic(),
          AssignStatement::getEntityTypeStatic());
  REQUIRE(relationships4->size() == 1);
  REQUIRE(relationships4->at(0)->getRelationshipType() ==
          FollowsStarRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships4->at(0)->getLeftHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships4->at(0)->getLeftHandEntity()->getEntityValue() == "1");
  REQUIRE(relationships4->at(0)->getRightHandEntity()->getEntityType() ==
          AssignStatement::getEntityTypeStatic());
  REQUIRE(*relationships4->at(0)->getRightHandEntity()->getEntityValue() ==
          "3");

  std::vector<Relationship *> *relationships5 =
      relationshipManager->getRelationshipsByTypes(
          CallsRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());

  REQUIRE(relationships5->size() == 1);
  REQUIRE(relationships5->at(0)->getRelationshipType() ==
          CallsRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships5->at(0)->getLeftHandEntity()->getEntityType() ==
          Procedure::getEntityTypeStatic());
  REQUIRE(*relationships5->at(0)->getLeftHandEntity()->getEntityValue() ==
          "Procedure");
  REQUIRE(relationships5->at(0)->getRightHandEntity()->getEntityType() ==
          Procedure::getEntityTypeStatic());
  REQUIRE(*relationships5->at(0)->getRightHandEntity()->getEntityValue() ==
          "Procedure2");

  std::vector<Relationship *> *relationships6 =
      relationshipManager->getRelationshipsByTypes(
          CallsRelationship::getRelationshipTypeStatic(),
          Procedure::getEntityTypeStatic(), Procedure::getEntityTypeStatic());

  REQUIRE(relationships6->size() == 1);
  REQUIRE(relationships6->at(0)->getRelationshipType() ==
          CallsRelationship::getRelationshipTypeStatic());
  REQUIRE(relationships6->at(0)->getLeftHandEntity()->getEntityType() ==
          Procedure::getEntityTypeStatic());
  REQUIRE(*relationships6->at(0)->getLeftHandEntity()->getEntityValue() ==
          "Procedure");
  REQUIRE(relationships6->at(0)->getRightHandEntity()->getEntityType() ==
          Procedure::getEntityTypeStatic());
  REQUIRE(*relationships6->at(0)->getRightHandEntity()->getEntityValue() ==
          "Procedure2");
}
