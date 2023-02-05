//
// Created by Aiken Wong on 4/2/23.
//
#include <stdexcept>

#include "catch.hpp"

#include <string>

#include "PKB/RelationshipHashkeyFactory.h"

#include "PKBStorageClasses/EntityClasses/Entity.h"

#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"




TEST_CASE("Assignment statement parent of read statement") {
    RelationshipHashkeyFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::PARENT;
    EntityType leftHandEntityType = EntityType::WHILE_STATEMENT;
    EntityType rightHandEntityType = EntityType::ASSIGN_STATEMENT;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "parent_While_Assign");
}
TEST_CASE("Assignment modifies variable") {
    RelationshipHashkeyFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::MODIFIES;
    EntityType leftHandEntityType = EntityType::ASSIGN_STATEMENT;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "modifies_Assign_Variable");
}

TEST_CASE("Procedure uses variable") {
    RelationshipHashkeyFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::USES;
    EntityType leftHandEntityType = EntityType::PROCEDURE;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "uses_Procedure_Variable");
}

TEST_CASE("Statement follows statement throws error") {
    RelationshipHashkeyFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::FOLLOWS;
    EntityType leftHandEntityType = EntityType::STATEMENT;
    EntityType rightHandEntityType = EntityType::STATEMENT;
    REQUIRE_THROWS(relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType));
}

TEST_CASE("Apply hash factory on Modifies Relationship") {
    Variable *variable = new Variable("x");
    AssignStatement *assignStatement = new AssignStatement(1);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(assignStatement, variable);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == "modifies_Assign_Variable");

    delete modifiesRelationship;
    delete assignStatement;
    delete variable;
}

TEST_CASE("Apply hash factory on Parent Relationship") {
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, readStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_While_Read");

    delete parentRelationship;
    delete readStatement;
    delete whileStatement;
}

TEST_CASE("Apply hash factory on Follows Relationship") {
    AssignStatement *assignStatement = new AssignStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(assignStatement, whileStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Assign_While");

    delete followsRelationship;
    delete assignStatement;
    delete whileStatement;
}

TEST_CASE("While Follows While") {
    WhileStatement *whileStatement1 = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(whileStatement1, whileStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_While_While");

    delete followsRelationship;
    delete whileStatement1;
    delete whileStatement2;
}

TEST_CASE("Call Follows While") {
    CallStatement *callStatement = new CallStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(callStatement, whileStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Call_While");

    delete followsRelationship;
    delete callStatement;
    delete whileStatement;
}

TEST_CASE("While Follows Call") {
    WhileStatement *whileStatement = new WhileStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(whileStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_While_Call");

    delete followsRelationship;
    delete whileStatement;
    delete callStatement;
}

TEST_CASE("If Follows Call") {
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_If_Call");

    delete followsRelationship;
    delete ifStatement;
    delete callStatement;
}

TEST_CASE("Call Follows If") {
    CallStatement *callStatement = new CallStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(callStatement, ifStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Call_If");

    delete followsRelationship;
    delete callStatement;
    delete ifStatement;
}

TEST_CASE("Assign Follows Call") {
    AssignStatement *assignStatement = new AssignStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(assignStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Assign_Call");

    delete followsRelationship;
    delete assignStatement;
    delete callStatement;
}

TEST_CASE("Assign Parent While") {
    AssignStatement *assignStatement = new AssignStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement, whileStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Assign_While");

    delete parentRelationship;
    delete assignStatement;
    delete whileStatement;
}

TEST_CASE("While Parent Assign") {
    WhileStatement *whileStatement = new WhileStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, assignStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_While_Assign");

    delete parentRelationship;
    delete whileStatement;
    delete assignStatement;
}

TEST_CASE("While Parent While") {
    WhileStatement *whileStatement1 = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement1, whileStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_While_While");

    delete parentRelationship;
    delete whileStatement1;
    delete whileStatement2;
}

TEST_CASE("If Parent While") {
    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement, whileStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_If_While");

    delete parentRelationship;
    delete ifStatement;
    delete whileStatement;
}

TEST_CASE("While Parent If") {
    WhileStatement *whileStatement = new WhileStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, ifStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_While_If");

    delete parentRelationship;
    delete whileStatement;
    delete ifStatement;
}

TEST_CASE("Call Parent While") {
    CallStatement *callStatement = new CallStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, whileStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Call_While");

    delete parentRelationship;
    delete callStatement;
    delete whileStatement;
}

TEST_CASE("While Parent Call") {
    WhileStatement *whileStatement = new WhileStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(whileStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_While_Call");

    delete parentRelationship;
    delete whileStatement;
    delete callStatement;
}

TEST_CASE("If Parent Call") {
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_If_Call");

    delete parentRelationship;
    delete ifStatement;
    delete callStatement;
}

TEST_CASE("Call Parent If") {
    CallStatement *callStatement = new CallStatement(1);
    IfStatement *ifStatement = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, ifStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Call_If");

    delete parentRelationship;
    delete callStatement;
    delete ifStatement;
}

TEST_CASE("Assign Parent Call") {
    AssignStatement *assignStatement = new AssignStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Assign_Call");

    delete parentRelationship;
    delete assignStatement;
    delete callStatement;
}

TEST_CASE("Call Parent Assign") {
    CallStatement *callStatement = new CallStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement, assignStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Call_Assign");

    delete parentRelationship;
    delete callStatement;
    delete assignStatement;
}

TEST_CASE("Assign Parent Assign") {
    AssignStatement *assignStatement1 = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(assignStatement1, assignStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Assign_Assign");

    delete parentRelationship;
    delete assignStatement1;
    delete assignStatement2;
}

TEST_CASE("Call Parent Call") {
    CallStatement *callStatement1 = new CallStatement(1);
    CallStatement *callStatement2 = new CallStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(callStatement1, callStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_Call_Call");

    delete parentRelationship;
    delete callStatement1;
    delete callStatement2;
}

TEST_CASE("If Parent If") {
    IfStatement *ifStatement1 = new IfStatement(1);
    IfStatement *ifStatement2 = new IfStatement(2);
    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement1, ifStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(parentRelationship);
    REQUIRE(hashKey == "parent_If_If");

    delete parentRelationship;
    delete ifStatement1;
    delete ifStatement2;
}

TEST_CASE("Print Follows Print") {
    PrintStatement *printStatement1 = new PrintStatement(1);
    PrintStatement *printStatement2 = new PrintStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement1, printStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Print_Print");

    delete followsRelationship;
    delete printStatement1;
    delete printStatement2;
}

TEST_CASE("Print Follows Read") {
    PrintStatement *printStatement = new PrintStatement(1);
    ReadStatement *readStatement = new ReadStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(printStatement, readStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Print_Read");

    delete followsRelationship;
    delete printStatement;
    delete readStatement;
}

TEST_CASE("Read Follows Print") {
    ReadStatement *readStatement = new ReadStatement(1);
    PrintStatement *printStatement = new PrintStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement, printStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Read_Print");

    delete followsRelationship;
    delete readStatement;
    delete printStatement;
}

TEST_CASE("Read Follows Read") {
    ReadStatement *readStatement1 = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement1, readStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Read_Read");

    delete followsRelationship;
    delete readStatement1;
    delete readStatement2;
}

TEST_CASE("If Follows If") {
    IfStatement *ifStatement1 = new IfStatement(1);
    IfStatement *ifStatement2 = new IfStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement1, ifStatement2);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_If_If");

    delete followsRelationship;
    delete ifStatement1;
    delete ifStatement2;
}

TEST_CASE("Read Follows Call") {
    ReadStatement *readStatement = new ReadStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    FollowsRelationship *followsRelationship = new FollowsRelationship(readStatement, callStatement);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Read_Call");

    delete followsRelationship;
    delete readStatement;
    delete callStatement;
}

TEST_CASE("Procedure Uses Variable") {
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");
    UsesRelationship *usesRelationship = new UsesRelationship(procedure, variable);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(usesRelationship);
    REQUIRE(hashKey == "uses_Procedure_Variable");

    delete usesRelationship;
    delete procedure;
    delete variable;
}

TEST_CASE("Assign Uses Variable") {
    AssignStatement *assignStatement = new AssignStatement(1);
    Variable *variable = new Variable("variable");
    UsesRelationship *usesRelationship = new UsesRelationship(assignStatement, variable);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(usesRelationship);
    REQUIRE(hashKey == "uses_Assign_Variable");

    delete usesRelationship;
    delete assignStatement;
    delete variable;
}

TEST_CASE("Procedure Modifies Variable") {
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(procedure, variable);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == "modifies_Procedure_Variable");

    delete modifiesRelationship;
    delete procedure;
    delete variable;
}

TEST_CASE("Read Modifies Variable") {
    ReadStatement *readStatement = new ReadStatement(1);
    Variable *variable = new Variable("variable");
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(readStatement, variable);
    RelationshipHashkeyFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(modifiesRelationship);
    REQUIRE(hashKey == "modifies_Read_Variable");

    delete modifiesRelationship;
    delete readStatement;
    delete variable;
}
