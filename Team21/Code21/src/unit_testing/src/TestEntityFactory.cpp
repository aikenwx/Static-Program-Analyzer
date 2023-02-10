#include <stdexcept>

#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityFactory.h"
#include "catch.hpp"

TEST_CASE("EntityFactory can instantiate") {
    EntityFactory entityFactory = EntityFactory();
}

TEST_CASE("EntityFactory can create an AssignmentStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::ASSIGN_STATEMENT, 1);
    AssignStatement expectedEntity = AssignStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a PrintStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::PRINT_STATEMENT, 1);
    PrintStatement expectedEntity = PrintStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a ReadStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::READ_STATEMENT, 1);
    ReadStatement expectedEntity = ReadStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a CallStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::CALL_STATEMENT, 1);
    CallStatement expectedEntity = CallStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a WhileStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::WHILE_STATEMENT, 1);
    WhileStatement expectedEntity = WhileStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create an IfStatement") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::IF_STATEMENT, 1);
    IfStatement expectedEntity = IfStatement(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a Procedure") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::PROCEDURE, "procedure");
    Procedure expectedEntity = Procedure("procedure");

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a Variable") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::VARIABLE, "variable");
    Variable expectedEntity = Variable("variable");

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("EntityFactory can create a Constant") {
    EntityFactory entityFactory = EntityFactory();

    Entity *actualEntity = entityFactory.createEntity(EntityType::CONSTANT, 1);
    Constant expectedEntity = Constant(1);

    REQUIRE(expectedEntity.equals(actualEntity));
    delete actualEntity;
}

TEST_CASE("Invalid Constant throws exception") {
    EntityFactory entityFactory = EntityFactory();
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::CONSTANT, "constant"), std::invalid_argument);
}

TEST_CASE("Invalid Statements throws exception") {
    EntityFactory entityFactory = EntityFactory();
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::ASSIGN_STATEMENT, "assign"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::PRINT_STATEMENT, "print"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::READ_STATEMENT, "read"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::CALL_STATEMENT, "call"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::WHILE_STATEMENT, "while"), std::invalid_argument);
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::IF_STATEMENT, "if"), std::invalid_argument);
}

TEST_CASE("Invalid Procedure throws exception") {
    EntityFactory entityFactory = EntityFactory();
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::PROCEDURE, 1), std::invalid_argument);
}

TEST_CASE("Invalid Variable throws exception") {
    EntityFactory entityFactory = EntityFactory();
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::VARIABLE, 1), std::invalid_argument);
}

TEST_CASE("Trying to create statement throws exception") {
    EntityFactory entityFactory = EntityFactory();
    REQUIRE_THROWS_AS(entityFactory.createEntity(EntityType::STATEMENT, 1), std::invalid_argument);
}
