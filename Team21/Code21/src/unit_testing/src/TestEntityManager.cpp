#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKB/EntityManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBtestHelpers.h"

TEST_CASE("EntityManager Instantiates") {
  auto entityManager = std::make_shared<EntityManager>();
}

TEST_CASE("EntityManager can store entity") {
  auto entityManager = std::make_shared<EntityManager>();

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));
}

TEST_CASE("EntityManager can retrieve entity") {
  auto entityManager = std::make_shared<EntityManager>();
  std::shared_ptr<ReadStatement> readStatement =
      std::make_shared<ReadStatement>(1);

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->equals(readStatement.get()));
}

TEST_CASE("EntityManager can retrieve mulitple entries") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement1 =
      std::make_shared<ReadStatement>(1);
  std::shared_ptr<ReadStatement> readStatement2 =
      std::make_shared<ReadStatement>(2);

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));
  entityManager->storeEntity(std::make_shared<ReadStatement>(2));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 2);
  REQUIRE(entities->at(0)->equals(readStatement1.get()));
  REQUIRE(entities->at(1)->equals(readStatement2.get()));
}

TEST_CASE("EntityManager returns empty vector if no entries") {
  auto entityManager = std::make_shared<EntityManager>();

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->empty());
}

TEST_CASE("EntityManager can retrieve multiple types") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement =
      std::make_shared<ReadStatement>(1);
  std::shared_ptr<AssignStatement> assignStatement =
      std::make_shared<AssignStatement>(2);

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));
  entityManager->storeEntity(std::make_shared<AssignStatement>(2));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->equals(readStatement.get()));

  entities =
      entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->equals(assignStatement.get()));
}

TEST_CASE("EntityManager can retrieve multiple entities of multiple types") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement1 =
      std::make_shared<ReadStatement>(1);
  std::shared_ptr<ReadStatement> readStatement2 =
      std::make_shared<ReadStatement>(2);
  std::shared_ptr<AssignStatement> assignStatement1 =
      std::make_shared<AssignStatement>(3);
  std::shared_ptr<AssignStatement> assignStatement2 =
      std::make_shared<AssignStatement>(4);

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));
  entityManager->storeEntity(std::make_shared<ReadStatement>(2));
  entityManager->storeEntity(std::make_shared<AssignStatement>(3));
  entityManager->storeEntity(std::make_shared<AssignStatement>(4));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 2);
  REQUIRE(entities->at(0)->equals(readStatement1.get()));
  REQUIRE(entities->at(1)->equals(readStatement2.get()));

  entities =
      entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 2);
  REQUIRE(entities->at(0)->equals(assignStatement1.get()));
  REQUIRE(entities->at(1)->equals(assignStatement2.get()));
}

TEST_CASE("EntityManager can retrieve all Statements") {
  // store all statements
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement1 =
      std::make_shared<ReadStatement>(ONE);
  std::shared_ptr<ReadStatement> readStatement2 =
      std::make_shared<ReadStatement>(TWO);
  std::shared_ptr<PrintStatement> printStatement1 =
      std::make_shared<PrintStatement>(THREE);
  std::shared_ptr<PrintStatement> printStatement2 =
      std::make_shared<PrintStatement>(FOUR);
  std::shared_ptr<AssignStatement> assignStatement1 =
      std::make_shared<AssignStatement>(FIVE);
  std::shared_ptr<AssignStatement> assignStatement2 =
      std::make_shared<AssignStatement>(SIX);
  std::shared_ptr<CallStatement> callStatement1 =
      std::make_shared<CallStatement>(SEVEN);
  std::shared_ptr<CallStatement> callStatement2 =
      std::make_shared<CallStatement>(EIGHT);
  std::shared_ptr<WhileStatement> whileStatement1 =
      std::make_shared<WhileStatement>(NINE);
  std::shared_ptr<WhileStatement> whileStatement2 =
      std::make_shared<WhileStatement>(TEN);
  std::shared_ptr<IfStatement> ifStatement1 =
      std::make_shared<IfStatement>(ELEVEN);
  std::shared_ptr<IfStatement> ifStatement2 =
      std::make_shared<IfStatement>(TWELVE);
  std::shared_ptr<Variable> variable =
      std::make_shared<Variable>(std::make_shared<std::string>("x"));
  std::shared_ptr<Procedure> procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("main"));

  entityManager->storeEntity(std::make_shared<ReadStatement>(ONE));
  entityManager->storeEntity(std::make_shared<ReadStatement>(TWO));
  entityManager->storeEntity(std::make_shared<PrintStatement>(THREE));
  entityManager->storeEntity(std::make_shared<PrintStatement>(FOUR));
  entityManager->storeEntity(std::make_shared<AssignStatement>(FIVE));
  entityManager->storeEntity(std::make_shared<AssignStatement>(SIX));
  entityManager->storeEntity(std::make_shared<CallStatement>(SEVEN));
  entityManager->storeEntity(std::make_shared<CallStatement>(EIGHT));
  entityManager->storeEntity(std::make_shared<WhileStatement>(NINE));
  entityManager->storeEntity(std::make_shared<WhileStatement>(TEN));
  entityManager->storeEntity(std::make_shared<IfStatement>(ELEVEN));
  entityManager->storeEntity(std::make_shared<IfStatement>(TWELVE));
  entityManager->storeEntity(
      std::make_shared<Variable>(std::make_shared<std::string>("x")));
  entityManager->storeEntity(
      std::make_shared<Procedure>(std::make_shared<std::string>("main")));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(Statement::getEntityTypeStatic());

  REQUIRE(entities->size() == 12);

  for (Entity *entity : *entities) {
    bool isStatement =
        entity->getEntityType() == ReadStatement::getEntityTypeStatic() ||
        entity->getEntityType() == PrintStatement::getEntityTypeStatic() ||
        entity->getEntityType() == AssignStatement::getEntityTypeStatic() ||
        entity->getEntityType() == CallStatement::getEntityTypeStatic() ||
        entity->getEntityType() == WhileStatement::getEntityTypeStatic() ||
        entity->getEntityType() == IfStatement::getEntityTypeStatic();

    REQUIRE(isStatement);
  }
}

TEST_CASE(
    "Retrieving statements from empty EntityManager returns empty vector") {
  auto entityManager = std::make_shared<EntityManager>();

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(Statement::getEntityTypeStatic());

  REQUIRE(entities->empty());
}

TEST_CASE("Storing duplicate entity handled correctly") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement =
      std::make_shared<ReadStatement>(1);

  entityManager->storeEntity(std::make_shared<ReadStatement>(1));
  entityManager->storeEntity(std::make_shared<ReadStatement>(1));

  std::vector<Entity *> *entities =
      entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

  REQUIRE(entities->size() == 1);
  REQUIRE(entities->at(0)->equals(readStatement.get()));
}

TEST_CASE("Can store and retrieve constant by constant value") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<Constant> constant = std::make_shared<Constant>(1);

  entityManager->storeEntity(std::make_shared<Constant>(1));

  auto key = EntityKey(&Constant::getEntityTypeStatic(), 1);

  auto *retrievedConstant =
      dynamic_cast<Constant *>(entityManager->getEntity(key));

  REQUIRE(retrievedConstant->equals(constant.get()));
}

TEST_CASE("Can store and retrieve variable by variable name") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<Variable> variable =
      std::make_shared<Variable>(std::make_shared<std::string>("x"));

  entityManager->storeEntity(
      std::make_shared<Variable>(std::make_shared<std::string>("x")));

  std::string word = "x";
  auto key = EntityKey(&Variable::getEntityTypeStatic(), &word);

  auto *retrievedVariable =
      dynamic_cast<Variable *>(entityManager->getEntity(key));

  REQUIRE(retrievedVariable->equals(variable.get()));
}

TEST_CASE("Can store and retrieve procedure by procedure name") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<Procedure> procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("main"));
  entityManager->storeEntity(
      std::make_shared<Procedure>(std::make_shared<std::string>("main")));

  std::string word = "main";
  auto key = EntityKey(&Procedure::getEntityTypeStatic(), &word);

  auto *retrievedProcedure =
      dynamic_cast<Procedure *>(entityManager->getEntity(key));

  REQUIRE(retrievedProcedure->equals(procedure.get()));
}

TEST_CASE("Can store and retrieve statement by statement number") {
  auto entityManager = std::make_shared<EntityManager>();

  std::shared_ptr<ReadStatement> readStatement =
      std::make_shared<ReadStatement>(1);
  entityManager->storeEntity(std::make_shared<ReadStatement>(1));

  auto key = EntityKey(&Statement::getEntityTypeStatic(), 1);

  auto *retrievedReadStatement =
      dynamic_cast<ReadStatement *>(entityManager->getEntity(key));

  REQUIRE(retrievedReadStatement->equals(readStatement.get()));
}
