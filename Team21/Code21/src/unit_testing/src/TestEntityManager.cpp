
#include "PKB/EntityManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "catch.hpp"

TEST_CASE("EntityManager Instantiates") {
    EntityManager *entityManager = new EntityManager();
    delete entityManager;
}

TEST_CASE("EntityManager can store entity") {
    EntityManager *entityManager = new EntityManager();

    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    entityManager->storeStatement(readStatement);

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve entity") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);

    entityManager->storeStatement(readStatement);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve mulitple entries") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);

    entityManager->storeStatement(readStatement1);
    entityManager->storeStatement(readStatement2);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1.get()));
    REQUIRE(entities->at(1)->equals(readStatement2.get()));

    delete entityManager;
}

TEST_CASE("EntityManager returns empty vector if no entries") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 0);

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve multiple types") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(2);
    entityManager->storeStatement(readStatement);
    entityManager->storeStatement(assignStatement);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement.get()));

    entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(assignStatement.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve multiple entities of multiple types") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);
    std::shared_ptr<AssignStatement> assignStatement1 = std::make_shared<AssignStatement>(3);
    std::shared_ptr<AssignStatement> assignStatement2 = std::make_shared<AssignStatement>(4);
    std::shared_ptr<Procedure> procedure1 = std::make_shared<Procedure>(new std::string("main"));
    std::shared_ptr<Procedure> procedure2 = std::make_shared<Procedure>(new std::string("main2"));

    entityManager->storeStatement(readStatement1);
    entityManager->storeStatement(readStatement2);
    entityManager->storeStatement(assignStatement1);
    entityManager->storeStatement(assignStatement2);
    entityManager->storeProcedure(procedure1);
    entityManager->storeProcedure(procedure2);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1.get()));
    REQUIRE(entities->at(1)->equals(readStatement2.get()));

    std::vector<Entity *> *entities2 = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);

    REQUIRE(entities2->size() == 2);
    REQUIRE(entities2->at(0)->equals(assignStatement1.get()));
    REQUIRE(entities2->at(1)->equals(assignStatement2.get()));

    std::vector<Entity *> *entities3 = entityManager->getEntitiesByType(EntityType::PROCEDURE);

    REQUIRE(entities3->size() == 2);
    REQUIRE(entities3->at(0)->equals(procedure1.get()));
    REQUIRE(entities3->at(1)->equals(procedure2.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve all Statements") {
    // store all statements
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);
    std::shared_ptr<PrintStatement> printStatement1 = std::make_shared<PrintStatement>(3);
    std::shared_ptr<PrintStatement> printStatement2 = std::make_shared<PrintStatement>(4);
    std::shared_ptr<AssignStatement> assignStatement1 = std::make_shared<AssignStatement>(5);
    std::shared_ptr<AssignStatement> assignStatement2 = std::make_shared<AssignStatement>(6);
    std::shared_ptr<CallStatement> callStatement1 = std::make_shared<CallStatement>(7);
    std::shared_ptr<CallStatement> callStatement2 = std::make_shared<CallStatement>(8);
    std::shared_ptr<WhileStatement> whileStatement1 = std::make_shared<WhileStatement>(9);
    std::shared_ptr<WhileStatement> whileStatement2 = std::make_shared<WhileStatement>(10);
    std::shared_ptr<IfStatement> ifStatement1 = std::make_shared<IfStatement>(11);
    std::shared_ptr<IfStatement> ifStatement2 = std::make_shared<IfStatement>(12);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("x"));
    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("main"));

    entityManager->storeStatement(readStatement1);
    entityManager->storeStatement(readStatement2);
    entityManager->storeStatement(printStatement1);
    entityManager->storeStatement(printStatement2);
    entityManager->storeStatement(assignStatement1);
    entityManager->storeStatement(assignStatement2);
    entityManager->storeStatement(callStatement1);
    entityManager->storeStatement(callStatement2);
    entityManager->storeStatement(whileStatement1);
    entityManager->storeStatement(whileStatement2);
    entityManager->storeStatement(ifStatement1);
    entityManager->storeStatement(ifStatement2);
    entityManager->storeVariable(variable);
    entityManager->storeProcedure(procedure);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::STATEMENT);

    REQUIRE(entities->size() == 12);

    for (Entity *entity : *entities) {
        bool isStatement = entity->getEntityType() == EntityType::READ_STATEMENT ||
                           entity->getEntityType() == EntityType::PRINT_STATEMENT ||
                           entity->getEntityType() == EntityType::ASSIGN_STATEMENT ||
                           entity->getEntityType() == EntityType::CALL_STATEMENT ||
                           entity->getEntityType() == EntityType::WHILE_STATEMENT ||
                           entity->getEntityType() == EntityType::IF_STATEMENT;

        REQUIRE(isStatement);
    }

    delete entityManager;
}

TEST_CASE("Retrieving statements from empty EntityManager returns empty vector") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::STATEMENT);

    REQUIRE(entities->size() == 0);

    delete entityManager;
}

TEST_CASE("Storing duplicate entity handled correctly") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(1);
    entityManager->storeStatement(readStatement);
    entityManager->storeStatement(readStatement2);
    
    REQUIRE(entityManager->getEntitiesByType(EntityType::READ_STATEMENT)->size() == 1);
    REQUIRE(entityManager->getEntitiesByType(EntityType::STATEMENT)->size() == 1);
    REQUIRE(entityManager->getEntitiesByType(EntityType::READ_STATEMENT)->at(0)->equals(readStatement.get()));   
    REQUIRE(entityManager->getStatementByStatementNumber(1)->equals(readStatement2.get()));

    delete entityManager;
}

TEST_CASE("Can store and retrieve constant by constant value") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Constant> constant = std::make_shared<Constant>(1);
    entityManager->storeConstant(constant);

    Constant *retrievedConstant = entityManager->getConstantByConstantValue(1);

    REQUIRE(retrievedConstant->equals(constant.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve variable by variable name") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("x"));
    entityManager->storeVariable(variable);

    Variable *retrievedVariable = entityManager->getVariableByVariableName("x");

    REQUIRE(retrievedVariable->equals(variable.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve procedure by procedure name") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("main"));
    entityManager->storeProcedure(procedure);

    Procedure *retrievedProcedure = entityManager->getProcedureByProcedureName("main");

    REQUIRE(retrievedProcedure->equals(procedure.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve statement by statement number") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    entityManager->storeStatement(readStatement);

    Statement *retrievedStatement = entityManager->getStatementByStatementNumber(1);

    REQUIRE(retrievedStatement->equals(readStatement.get()));
    delete entityManager;
}

