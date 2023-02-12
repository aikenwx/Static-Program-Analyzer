//
// Created by Aiken Wong on 4/2/23.
//
#include <stdexcept>
#include <string>

#include "PKB/RelationshipSynonymHashkeyGenerator.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "catch.hpp"

enum class stubEntityType {
    STUB_ASSIGN_STATEMENT = 0,
    STUB_CALL_STATEMENT = 1,
    STUB_IF_STATEMENT = 2,
    STUB_PRINT_STATEMENT = 3,
    STUB_READ_STATEMENT = 4,
    STUB_WHILE_STATEMENT = 5,
    STUB_CONSTANT = 6,
    STUB_VARIABLE = 7,
    STUB_PROCEDURE = 8,
    STUB_STATEMENT = 9,
};
enum stubRelationshipType {
    STUB_MODIFIES = 0,
    STUB_USES = 1,
    STUB_PARENT = 2,
    STUB_FOLLOWS = 3,
    STUB_PARENT_STAR = 4,
    STUB_FOLLOWS_STAR = 5,
};

int stubBase = 10;

TEST_CASE("Assignment statement parent of read statement") {
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::PARENT;
    EntityType leftHandEntityType = EntityType::WHILE_STATEMENT;
    EntityType rightHandEntityType = EntityType::ASSIGN_STATEMENT;
    int hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);
}
TEST_CASE("Assignment modifies variable") {
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::MODIFIES;
    EntityType leftHandEntityType = EntityType::ASSIGN_STATEMENT;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    int hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_MODIFIES * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_VARIABLE);
}

TEST_CASE("Procedure uses variable") {
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::USES;
    EntityType leftHandEntityType = EntityType::PROCEDURE;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    int hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE((int)stubRelationshipType::STUB_USES * stubBase * stubBase + (int)stubEntityType::STUB_PROCEDURE * stubBase + (int)stubEntityType::STUB_VARIABLE);
}

TEST_CASE("Apply hash factory on Modifies Relationship") {
    Variable *variable = new Variable(new std::string("x"));
    AssignStatement *assignStatement = new AssignStatement(1);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(assignStatement, variable);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_MODIFIES * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_VARIABLE);
    delete modifiesRelationship;
}

TEST_CASE("Apply hash factory on Parent Relationship") {
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, readStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_READ_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Apply hash factory on Follows Relationship") {
    AssignStatement *assignStatement = new AssignStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(assignStatement, whileStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("While Follows While") {
    WhileStatement *whileStatement1 = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(whileStatement1, whileStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Call Follows While") {
    CallStatement *callStatement = new CallStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(callStatement, whileStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("While Follows Call") {
    WhileStatement *whileStatement = new WhileStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(whileStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("If Follows Call") {
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Call Follows If") {
    CallStatement *callStatement = new CallStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(callStatement, ifStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Assign Follows Call") {
    AssignStatement *assignStatement = new AssignStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(assignStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Assign Parent While") {
    AssignStatement *assignStatement = new AssignStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement, whileStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("While Parent Assign") {
    WhileStatement *whileStatement = new WhileStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("While Parent While") {
    WhileStatement *whileStatement1 = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement1, whileStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("If Parent While") {
    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement, whileStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("While Parent If") {
    WhileStatement *whileStatement = new WhileStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, ifStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Call Parent While") {
    CallStatement *callStatement = new CallStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, whileStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("While Parent Call") {
    WhileStatement *whileStatement = new WhileStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("If Parent Call") {
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Call Parent If") {
    CallStatement *callStatement = new CallStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, ifStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Assign Parent Call") {
    AssignStatement *assignStatement = new AssignStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Call Parent Assign") {
    CallStatement *callStatement = new CallStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, assignStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Assign Parent Assign") {
    AssignStatement *assignStatement1 = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement1, assignStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;

    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Call Parent Call") {
    CallStatement *callStatement1 = new CallStatement(1);
    CallStatement *callStatement2 = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement1, callStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("If Parent If") {
    IfStatement *ifStatement1 = new IfStatement(1);
    IfStatement *ifStatement2 = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement1, ifStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete parentRelationship;
}

TEST_CASE("Print Follows Print") {
    PrintStatement *printStatement1 = new PrintStatement(1);
    PrintStatement *printStatement2 = new PrintStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement1, printStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Print Follows Read") {
    PrintStatement *printStatement = new PrintStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT * stubBase + (int)stubEntityType::STUB_READ_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Read Follows Print") {
    ReadStatement *readStatement = new ReadStatement(1);
    PrintStatement *printStatement = new PrintStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement, printStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_READ_STATEMENT * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Read Follows Read") {
    ReadStatement *readStatement1 = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement1, readStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_READ_STATEMENT * stubBase + (int)stubEntityType::STUB_READ_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("If Follows If") {
    IfStatement *ifStatement1 = new IfStatement(1);
    IfStatement *ifStatement2 = new IfStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement1, ifStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Read Follows Call") {
    ReadStatement *readStatement = new ReadStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_FOLLOWS * stubBase * stubBase + (int)stubEntityType::STUB_READ_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete followsRelationship;
}

TEST_CASE("Procedure Uses Variable") {
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(usesRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_USES * stubBase * stubBase + (int)stubEntityType::STUB_PROCEDURE * stubBase + (int)stubEntityType::STUB_VARIABLE);

    delete usesRelationship;
}

TEST_CASE("Assign Uses Variable") {
    AssignStatement *assignStatement = new AssignStatement(1);
    Variable *variable = new Variable(new std::string("variable"));
    UsesRelationship *usesRelationship = new UsesRelationship(assignStatement, variable);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(usesRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_USES * stubBase * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT * stubBase + (int)stubEntityType::STUB_VARIABLE);

    delete usesRelationship;
}

TEST_CASE("Procedure Modifies Variable") {
    Procedure *procedure = new Procedure(new std::string("procedure"));
    Variable *variable = new Variable(new std::string("variable"));
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_MODIFIES * stubBase * stubBase + (int)stubEntityType::STUB_PROCEDURE * stubBase + (int)stubEntityType::STUB_VARIABLE);

    delete modifiesRelationship;
}

TEST_CASE("Read Modifies Variable") {
    ReadStatement *readStatement = new ReadStatement(1);
    Variable *variable = new Variable(new std::string("variable"));
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(readStatement, variable);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_MODIFIES * stubBase * stubBase + (int)stubEntityType::STUB_READ_STATEMENT * stubBase + (int)stubEntityType::STUB_VARIABLE);

    delete modifiesRelationship;
}

TEST_CASE("While ParentStar While") {
    WhileStatement *whileStatement1 = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement1, whileStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("While ParentStar If") {
    WhileStatement *whileStatement = new WhileStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, ifStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("While ParentStar Call") {
    WhileStatement *whileStatement = new WhileStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("While ParentStar Assign") {
    WhileStatement *whileStatement = new WhileStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, assignStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("While ParentStar Print") {
    WhileStatement *whileStatement = new WhileStatement(1);
    PrintStatement *printStatement = new PrintStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, printStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("While ParentStar Read") {
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(whileStatement, readStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_WHILE_STATEMENT * stubBase + (int)stubEntityType::STUB_READ_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("If ParentStar If") {
    IfStatement *ifStatement1 = new IfStatement(1);
    IfStatement *ifStatement2 = new IfStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(ifStatement1, ifStatement2);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_IF_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("If ParentStar Call") {
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(ifStatement, callStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_CALL_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("If ParentStar Assign") {
    IfStatement *ifStatement = new IfStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(ifStatement, assignStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_ASSIGN_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("If ParentStar Print") {
    IfStatement *ifStatement = new IfStatement(1);
    PrintStatement *printStatement = new PrintStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(ifStatement, printStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_PRINT_STATEMENT);

    delete parentStarRelationship;
}

TEST_CASE("If ParentStar Read") {
    IfStatement *ifStatement = new IfStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    ParentStarRelationship *parentStarRelationship = new ParentStarRelationship(ifStatement, readStatement);
    RelationshipSynonymHashkeyGenerator relationshipHashFactory;
    int hashKey = relationshipHashFactory.getHashKey(parentStarRelationship);
    REQUIRE(hashKey == (int)stubRelationshipType::STUB_PARENT_STAR * stubBase * stubBase + (int)stubEntityType::STUB_IF_STATEMENT * stubBase + (int)stubEntityType::STUB_READ_STATEMENT);

    delete parentStarRelationship;
}