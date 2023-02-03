#include <stdexcept>

#include "catch.hpp"

#include "PKBStorageClasses/EntityFactory.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"


TEST_CASE("EntityFactory can instantiate") {
    EntityFactory* entityFactory = new EntityFactory();
    delete entityFactory;
}

TEST_CASE("EntityFactory can create an AssignmentStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::ASSIGN_STATEMENT, 1);
    AssignStatement *expectedEntity = new AssignStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create a PrintStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::PRINT_STATEMENT, 1);
    PrintStatement *expectedEntity = new PrintStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create a ReadStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::READ_STATEMENT, 1);
    ReadStatement *expectedEntity = new ReadStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create a CallStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::CALL_STATEMENT, 1);
    CallStatement *expectedEntity = new CallStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create a WhileStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::WHILE_STATEMENT, 1);
    WhileStatement *expectedEntity = new WhileStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create an IfStatement") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::IF_STATEMENT, 1);
    IfStatement *expectedEntity = new IfStatement(1);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a Procedure") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::PROCEDURE, "procedure");
    Procedure *expectedEntity = new Procedure("procedure");

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;

}

TEST_CASE("EntityFactory can create a Variable") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::VARIABLE, "variable");
    Variable *expectedEntity = new Variable("variable");

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a Constant") {
    EntityFactory* entityFactory = new EntityFactory();

    Entity *actualEntity = entityFactory->createEntity(EntityType::CONSTANT, 5);
    Constant *expectedEntity = new Constant(5);

    REQUIRE(expectedEntity->equals(actualEntity));
    delete entityFactory;
    delete expectedEntity;
    delete actualEntity;
}

TEST_CASE("Invalid Constant throws exception") {
    EntityFactory* entityFactory = new EntityFactory();
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::CONSTANT, "constant"), std::invalid_argument);
    delete entityFactory;
}

TEST_CASE("Invalid Statements throws exception") {
    EntityFactory* entityFactory = new EntityFactory();
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::ASSIGN_STATEMENT, "assign"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::PRINT_STATEMENT, "print"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::READ_STATEMENT, "read"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::CALL_STATEMENT, "call"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::WHILE_STATEMENT, "while"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::IF_STATEMENT, "if"), std::invalid_argument);

    delete entityFactory;
}

TEST_CASE("Invalid Procedure throws exception") {
    EntityFactory* entityFactory = new EntityFactory();
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::PROCEDURE, 1), std::invalid_argument);
    delete entityFactory;
}

TEST_CASE("Invalid Variable throws exception") {
    EntityFactory* entityFactory = new EntityFactory();
    REQUIRE_THROWS_AS(entityFactory->createEntity(EntityType::VARIABLE, 1), std::invalid_argument);
    delete entityFactory;
}
