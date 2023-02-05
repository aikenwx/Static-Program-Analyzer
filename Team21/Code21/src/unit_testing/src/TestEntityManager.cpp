
#include "catch.hpp"

#include "PKB/EntityManager.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"

TEST_CASE("EntityManager Instantiates") {
    EntityManager *entityManager = new EntityManager();
    delete entityManager;
}

TEST_CASE("EntityManager can store entity") {
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement = new ReadStatement(1);

    entityManager->storeEntity(readStatement);

    delete entityManager;
    delete readStatement;
}

TEST_CASE("EntityManager can retrieve entity") {
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement = new ReadStatement(1);

    entityManager->storeEntity(readStatement);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement));

    delete entityManager;
    delete readStatement;
    delete entities;
}

TEST_CASE("EntityManager can retrieve mulitple entries") {
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement1 = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    entityManager->storeEntity(readStatement1);
    entityManager->storeEntity(readStatement2);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1));
    REQUIRE(entities->at(1)->equals(readStatement2));

    delete entityManager;
    delete readStatement1;
    delete readStatement2;
    delete entities;

}

TEST_CASE("EntityManager returns empty vector if no entries") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 0);

    delete entityManager;
    delete entities;

}

TEST_CASE("EntityManager can retrieve multiple types") {
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement = new ReadStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);

    entityManager->storeEntity(readStatement);
    entityManager->storeEntity(assignStatement);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement));

    entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(assignStatement));

    delete entityManager;
    delete readStatement;
    delete assignStatement;
    delete entities;
}


TEST_CASE("EntityManager can retrieve multiple entities of multiple types") {
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement1 = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    AssignStatement *assignStatement1 = new AssignStatement(3);
    AssignStatement *assignStatement2 = new AssignStatement(4);
    Procedure *procedure1 = new Procedure("procedure1");
    Procedure *procedure2 = new Procedure("procedure2");

    entityManager->storeEntity(readStatement1);
    entityManager->storeEntity(readStatement2);
    entityManager->storeEntity(assignStatement1);
    entityManager->storeEntity(assignStatement2);
    entityManager->storeEntity(procedure1);
    entityManager->storeEntity(procedure2);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::READ_STATEMENT);

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1));
    REQUIRE(entities->at(1)->equals(readStatement2));

    std::vector<Entity *> *entities2 = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);

    REQUIRE(entities2->size() == 2);
    REQUIRE(entities2->at(0)->equals(assignStatement1));
    REQUIRE(entities2->at(1)->equals(assignStatement2));

    std::vector<Entity *> *entities3 = entityManager->getEntitiesByType(EntityType::PROCEDURE);

    REQUIRE(entities3->size() == 2);
    REQUIRE(entities3->at(0)->equals(procedure1));
    REQUIRE(entities3->at(1)->equals(procedure2));

    delete entityManager;
    delete readStatement1;
    delete readStatement2;
    delete assignStatement1;
    delete assignStatement2;
    delete procedure1;
    delete procedure2;
    delete entities;
    delete entities2;
    delete entities3;
}

TEST_CASE("EntityManager can retrieve all Statements") {
    // store all statements
    EntityManager *entityManager = new EntityManager();
    ReadStatement *readStatement1 = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement1 = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);
    AssignStatement *assignStatement1 = new AssignStatement(5);
    AssignStatement *assignStatement2 = new AssignStatement(6);
    CallStatement *callStatement1 = new CallStatement(7);
    CallStatement *callStatement2 = new CallStatement(8);
    WhileStatement *whileStatement1 = new WhileStatement(9);
    WhileStatement *whileStatement2 = new WhileStatement(10);
    IfStatement *ifStatement1 = new IfStatement(11);
    IfStatement *ifStatement2 = new IfStatement(12);
    Variable *variable = new Variable("variable");
    Procedure *procedure = new Procedure("procedure");

    entityManager->storeEntity(readStatement1);
    entityManager->storeEntity(readStatement2);
    entityManager->storeEntity(printStatement1);
    entityManager->storeEntity(printStatement2);
    entityManager->storeEntity(assignStatement1);
    entityManager->storeEntity(assignStatement2);
    entityManager->storeEntity(callStatement1);
    entityManager->storeEntity(callStatement2);
    entityManager->storeEntity(whileStatement1);
    entityManager->storeEntity(whileStatement2);
    entityManager->storeEntity(ifStatement1);
    entityManager->storeEntity(ifStatement2);
    entityManager->storeEntity(variable);
    entityManager->storeEntity(procedure);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::STATEMENT);

    REQUIRE(entities->size() == 12);

    for (Entity *entity: *entities) {
        bool isStatement = entity->getEntityType() == EntityType::READ_STATEMENT ||
                           entity->getEntityType() == EntityType::PRINT_STATEMENT ||
                           entity->getEntityType() == EntityType::ASSIGN_STATEMENT ||
                           entity->getEntityType() == EntityType::CALL_STATEMENT ||
                           entity->getEntityType() == EntityType::WHILE_STATEMENT ||
                           entity->getEntityType() == EntityType::IF_STATEMENT;

        REQUIRE(isStatement);
    }

    delete entityManager;
    delete readStatement1;
    delete readStatement2;
    delete printStatement1;
    delete printStatement2;
    delete assignStatement1;
    delete assignStatement2;
    delete callStatement1;
    delete callStatement2;
    delete whileStatement1;
    delete whileStatement2;
    delete ifStatement1;
    delete ifStatement2;
    delete variable;
    delete procedure;
    delete entities;
}

TEST_CASE("Retrieving statements from empty EntityManager returns empty vector") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::STATEMENT);

    REQUIRE(entities->size() == 0);

    delete entityManager;
    delete entities;
}
