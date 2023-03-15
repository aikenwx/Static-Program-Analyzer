
#include <catch2/catch_test_macros.hpp>
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

TEST_CASE("Relationships can instantiate") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    Procedure *procedure2 = new Procedure(new std::string("procedure2"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    CallsRelationship *callsRelationship = new CallsRelationship(procedure, procedure2);
    CallsStarRelationship *callsStarRelationship = new CallsStarRelationship(procedure, procedure2);

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure2, variable2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);

    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    FollowsStarRelationship *followsStarRelationship = new FollowsStarRelationship(printStatement, readStatement);

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;
    delete parentStarRelationship;
    delete followsStarRelationship;
    delete printStatement;
    delete readStatement;
    delete whileStatement;
    delete assignStatement;
    delete procedure;
    delete variable;
    delete procedure2;
    delete variable2;
    delete callsRelationship;
    delete callsStarRelationship;
}

TEST_CASE("Relationships contain the correct left entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    Procedure *procedure2 = new Procedure(new std::string("procedure2"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    CallsRelationship *callsRelationship = new CallsRelationship(procedure, procedure2);
    CallsStarRelationship *callsStarRelationship = new CallsStarRelationship(procedure, procedure2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure2, variable2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    FollowsStarRelationship *followsStarRelationship = new FollowsStarRelationship(printStatement, readStatement);

    REQUIRE(parentRelationship->containsEntityOnLeftHand(whileStatement));
    REQUIRE(usesRelationship->containsEntityOnLeftHand(procedure));
    REQUIRE(modifiesRelationship->containsEntityOnLeftHand(procedure2));
    REQUIRE(followsRelationship->containsEntityOnLeftHand(printStatement));
    REQUIRE(parentStarRelationship->containsEntityOnLeftHand(whileStatement));
    REQUIRE(followsStarRelationship->containsEntityOnLeftHand(printStatement));
    REQUIRE(callsRelationship->containsEntityOnLeftHand(procedure));
    REQUIRE(callsStarRelationship->containsEntityOnLeftHand(procedure));

    delete callsRelationship;
    delete callsStarRelationship;
    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;
    delete parentStarRelationship;
    delete followsStarRelationship;
    delete printStatement;
    delete readStatement;
    delete whileStatement;
    delete assignStatement;
    delete procedure;
    delete variable;
    delete procedure2;
    delete variable2;
}

TEST_CASE("Relationships contain the correct right entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    Procedure *procedure2 = new Procedure(new std::string("procedure2"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    CallsRelationship *callsRelationship = new CallsRelationship(procedure, procedure2);
    CallsStarRelationship *callsStarRelationship = new CallsStarRelationship(procedure, procedure2);

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure2, variable2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    FollowsStarRelationship *followsStarRelationship = new FollowsStarRelationship(printStatement, readStatement);

    REQUIRE(callsRelationship->containsEntityOnRightHand(procedure2));
    REQUIRE(callsStarRelationship->containsEntityOnRightHand(procedure2));
    REQUIRE(parentRelationship->containsEntityOnRightHand(assignStatement));
    REQUIRE(usesRelationship->containsEntityOnRightHand(variable));
    REQUIRE(modifiesRelationship->containsEntityOnRightHand(variable2));
    REQUIRE(followsRelationship->containsEntityOnRightHand(readStatement));

    delete callsRelationship;
    delete callsStarRelationship;
    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;
    delete printStatement;
    delete readStatement;
    delete whileStatement;
    delete assignStatement;
    delete procedure;
    delete variable;
    delete procedure2;
    delete variable2;
    delete parentStarRelationship;
    delete followsStarRelationship;
}

TEST_CASE("Relationships contain the wrong left entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    Procedure *procedure2 = new Procedure(new std::string("procedure2"));
    Variable *variable2 = new Variable(new std::string("variable2"));
    CallsRelationship *callsRelationship = new CallsRelationship(procedure, procedure2);
    CallsStarRelationship *callsStarRelationship = new CallsStarRelationship(procedure, procedure2);

    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure2, variable2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    FollowsStarRelationship *followsStarRelationship = new FollowsStarRelationship(printStatement, readStatement);

    REQUIRE(!parentRelationship->containsEntityOnLeftHand(assignStatement));
    REQUIRE(!usesRelationship->containsEntityOnLeftHand(variable));
    REQUIRE(!modifiesRelationship->containsEntityOnLeftHand(variable2));
    REQUIRE(!followsRelationship->containsEntityOnLeftHand(readStatement));
    REQUIRE(!parentStarRelationship->containsEntityOnLeftHand(assignStatement));
    REQUIRE(!followsStarRelationship->containsEntityOnLeftHand(readStatement));
    REQUIRE(!callsRelationship->containsEntityOnLeftHand(procedure2));
    REQUIRE(!callsStarRelationship->containsEntityOnLeftHand(procedure2));

    delete callsRelationship;
    delete callsStarRelationship;
    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;
    delete parentStarRelationship;
    delete followsStarRelationship;
    delete printStatement;
    delete readStatement;
    delete whileStatement;
    delete assignStatement;
    delete procedure;
    delete variable;
    delete procedure2;
    delete variable2;
}

TEST_CASE("Relationships contain the wrong right entity") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    WhileStatement *whileStatement = new WhileStatement(4);
    AssignStatement *assignStatement = new AssignStatement(5);
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    Procedure *procedure2 = new Procedure(new std::string("procedure2"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    CallsRelationship *callsRelationship = new CallsRelationship(procedure, procedure2);
    CallsStarRelationship *callsStarRelationship = new CallsStarRelationship(procedure, procedure2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure2, variable2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    FollowsStarRelationship *followsStarRelationship = new FollowsStarRelationship(printStatement, readStatement);

    REQUIRE(!parentRelationship->containsEntityOnRightHand(whileStatement));
    REQUIRE(!usesRelationship->containsEntityOnRightHand(procedure));
    REQUIRE(!modifiesRelationship->containsEntityOnRightHand(procedure2));
    REQUIRE(!followsRelationship->containsEntityOnRightHand(printStatement));
    REQUIRE(!parentStarRelationship->containsEntityOnRightHand(whileStatement));
    REQUIRE(!followsStarRelationship->containsEntityOnRightHand(printStatement));
    REQUIRE(!callsRelationship->containsEntityOnRightHand(procedure));
    REQUIRE(!callsStarRelationship->containsEntityOnRightHand(procedure));

    delete parentRelationship;
    delete usesRelationship;
    delete modifiesRelationship;
    delete followsRelationship;
    delete parentStarRelationship;
    delete followsStarRelationship;
    delete callsRelationship;
    delete callsStarRelationship;
    delete printStatement;
    delete readStatement;
    delete whileStatement;
    delete assignStatement;
    delete procedure;
    delete variable;
    delete procedure2;
    delete variable2;
}

TEST_CASE("Instantiating a ModifiesRelationship with a non-Procedure or non-Statement as LHS throws an exception") {
    Variable *variable = new Variable(new std::string("variable"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    REQUIRE_THROWS_AS(new ModifiesRelationship(variable2, variable), std::invalid_argument);

    delete variable2;
    delete variable;
}

TEST_CASE("Instantiating a UsesRelationship with a non-Procedure or non-Statement as LHS entity throws an exception") {
    Variable *variable = new Variable(new std::string("variable"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    REQUIRE_THROWS_AS(new UsesRelationship(variable2, variable), std::invalid_argument);

    delete variable2;
    delete variable;
}
