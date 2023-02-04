
#include "catch.hpp"

#include "PKB/EntityManager.h"

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
