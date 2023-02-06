
#include <stdexcept>

#include "catch.hpp"

#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"


TEST_CASE("Relationships can instantiate") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete whileStatement;
    delete procedure;
    delete variable;
}

TEST_CASE("Relationships contain the correct left entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    REQUIRE(parentRelationship->containsEntityOnLeftHand(whileStatement));
    REQUIRE(usesRelationship->containsEntityOnLeftHand(procedure));
    REQUIRE(modifiesRelationship->containsEntityOnLeftHand(procedure));
    REQUIRE(followsRelationship->containsEntityOnLeftHand(printStatement));

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete whileStatement;
    delete procedure;
    delete variable;
}

TEST_CASE("Relationships contain the correct right entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    REQUIRE(parentRelationship->containsEntityOnRightHand(assignStatement));
    REQUIRE(usesRelationship->containsEntityOnRightHand(variable));
    REQUIRE(modifiesRelationship->containsEntityOnRightHand(variable));
    REQUIRE(followsRelationship->containsEntityOnRightHand(readStatement));

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete whileStatement;
    delete procedure;
    delete variable;
}

TEST_CASE("Relationships contain the wrong left entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    REQUIRE(!parentRelationship->containsEntityOnLeftHand(assignStatement));
    REQUIRE(!usesRelationship->containsEntityOnLeftHand(variable));
    REQUIRE(!modifiesRelationship->containsEntityOnLeftHand(variable));
    REQUIRE(!followsRelationship->containsEntityOnLeftHand(readStatement));

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete whileStatement;
    delete procedure;
    delete variable;
}

TEST_CASE("Relationships contain the wrong right entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    REQUIRE(!parentRelationship->containsEntityOnRightHand(whileStatement));
    REQUIRE(!usesRelationship->containsEntityOnRightHand(procedure));
    REQUIRE(!modifiesRelationship->containsEntityOnRightHand(procedure));
    REQUIRE(!followsRelationship->containsEntityOnRightHand(printStatement));

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete whileStatement;
    delete procedure;
    delete variable;
}

TEST_CASE("Instantiating a ModifiesRelationship with a non-Procedure or non-Statement as LHS throws an exception") {
    Variable *variable = new Variable("variable");
    Variable *variable2 = new Variable("variable2");

    REQUIRE_THROWS_AS(new ModifiesRelationship(variable2, variable), std::invalid_argument);

    delete variable2;
    delete variable;
}

TEST_CASE("Instantiating a UsesRelationship with a non-Procedure or non-Statement as LHS entity throws an exception") {
    Variable *variable = new Variable("variable");
    Variable *variable2 = new Variable("variable2");

    REQUIRE_THROWS_AS(new UsesRelationship(variable2, variable), std::invalid_argument);

    delete variable2;
    delete variable;
}
