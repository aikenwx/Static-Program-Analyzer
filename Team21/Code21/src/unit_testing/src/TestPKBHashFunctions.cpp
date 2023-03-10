#include <stdexcept>
#include <string>

#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBtestHelpers.h"
#include "catch.hpp"

// std::string getTestHash(std::string firstEntityValue, testEntityType firstEntityType, std::string secondEntityValue,
//                         testEntityType secondEntityType, testRelationshipType relationshipType) {
//     std::string relationshipTypeHash = std::to_string((int)relationshipType * testBase * testBase + (int)firstEntityType * testBase + (int)secondEntityType);

//     return firstEntityValue + "_" + relationshipTypeHash + "_" + secondEntityValue;
// }

TEST_CASE("Test Entity Key uniqueness and correctness") {
    REQUIRE(EntityKey(&AssignStatement::getEntityTypeStatic(), 1) == EntityKey(&AssignStatement::getEntityTypeStatic(), 1));
    REQUIRE(!(EntityKey(&AssignStatement::getEntityTypeStatic(), 1) == EntityKey(&AssignStatement::getEntityTypeStatic(), 2)));

    REQUIRE(std::hash<EntityKey>()(EntityKey(&AssignStatement::getEntityTypeStatic(), 1)) == std::hash<EntityKey>()(EntityKey(&AssignStatement::getEntityTypeStatic(), 1)));

    REQUIRE(std::hash<EntityKey>()(EntityKey(&AssignStatement::getEntityTypeStatic(), 1)) != std::hash<EntityKey>()(EntityKey(&AssignStatement::getEntityTypeStatic(), 2)));

    REQUIRE(EntityKey(&CallStatement::getEntityTypeStatic(), 1) == EntityKey(&CallStatement::getEntityTypeStatic(), 1));
    REQUIRE(!(EntityKey(&CallStatement::getEntityTypeStatic(), 1) == EntityKey(&AssignStatement::getEntityTypeStatic(), 1)));
}

// TEST_CASE("Assignment Statement Follows Assigment Statement Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     AssignStatement *assignStatement = new AssignStatement(1);
//     AssignStatement *assignStatement2 = new AssignStatement(2);
//     Relationship *relationship = new FollowsRelationship(assignStatement, assignStatement2);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS));
//     delete relationship;
//     delete assignStatement;
//     delete assignStatement2;
// }

// TEST_CASE("Call Statement Follows Call statement Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     CallStatement *callStatement = new CallStatement(1);
//     CallStatement *callStatement2 = new CallStatement(2);
//     Relationship *relationship = new FollowsRelationship(callStatement, callStatement2);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_FOLLOWS, (EntityType)testEntityType::TEST_STATEMENT, &ONE_STRING_TEST_VALUE, (EntityType)testEntityType::TEST_STATEMENT, &TWO_STRING_TEST_VALUE) == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS));
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS));

//     delete relationship;
//     delete callStatement;
//     delete callStatement2;
// }

// TEST_CASE("If Statement Follows if Statement Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     IfStatement *ifStatement = new IfStatement(1);
//     IfStatement *ifStatement2 = new IfStatement(2);
//     Relationship *relationship = new FollowsRelationship(ifStatement, ifStatement2);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_FOLLOWS, (EntityType)testEntityType::TEST_STATEMENT, &ONE_STRING_TEST_VALUE, (EntityType)testEntityType::TEST_STATEMENT, &TWO_STRING_TEST_VALUE) == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS));
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS));

//     delete relationship;
//     delete ifStatement;
//     delete ifStatement2;
// }

// TEST_CASE("While Statement ParentStar Call Statement Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     WhileStatement *whileStatement = new WhileStatement(1);
//     CallStatement *callStatement = new CallStatement(2);
//     Relationship *relationship = new ParentStarRelationship(whileStatement, callStatement);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_PARENT_STAR, (EntityType)testEntityType::TEST_STATEMENT, &ONE_STRING_TEST_VALUE, (EntityType)testEntityType::TEST_STATEMENT, &TWO_STRING_TEST_VALUE) == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_PARENT_STAR));
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_PARENT_STAR));
//     delete relationship;
//     delete whileStatement;
//     delete callStatement;
// }

// TEST_CASE("Print Statement FollowsStar Assignment Statement Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     PrintStatement *printStatement = new PrintStatement(1);
//     AssignStatement *assignStatement = new AssignStatement(2);
//     Relationship *relationship = new FollowsStarRelationship(printStatement, assignStatement);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_FOLLOWS_STAR, (EntityType)testEntityType::TEST_STATEMENT, &ONE_STRING_TEST_VALUE, (EntityType)testEntityType::TEST_STATEMENT, &TWO_STRING_TEST_VALUE) == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS_STAR));
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, TWO_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, testRelationshipType::TEST_FOLLOWS_STAR));
//     delete relationship;
//     delete printStatement;
//     delete assignStatement;
// }

// TEST_CASE("Procedure Uses Variable Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     Procedure *procedure = new Procedure(new std::string(PROCEDURE_TEST_VALUE));
//     Variable *variable = new Variable(new std::string(VARIABLE_TEST_VALUE));
//     Relationship *relationship = new UsesRelationship(procedure, variable);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_USES, (EntityType)testEntityType::TEST_PROCEDURE, &PROCEDURE_TEST_VALUE, (EntityType)testEntityType::TEST_VARIABLE, &VARIABLE_TEST_VALUE) == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, VARIABLE_TEST_VALUE, testEntityType::TEST_VARIABLE, testRelationshipType::TEST_USES));
//     REQUIRE(hashkey == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, VARIABLE_TEST_VALUE, testEntityType::TEST_VARIABLE, testRelationshipType::TEST_USES));
//     delete relationship;
//     delete procedure;
//     delete variable;
// }

// TEST_CASE("Assignment Statement Modifies Variable Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     AssignStatement *assignStatement = new AssignStatement(1);
//     Variable *variable = new Variable(new std::string(VARIABLE_TEST_VALUE));
//     Relationship *relationship = new ModifiesRelationship(assignStatement, variable);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_MODIFIES, (EntityType)testEntityType::TEST_STATEMENT, &ONE_STRING_TEST_VALUE, (EntityType)testEntityType::TEST_VARIABLE, &VARIABLE_TEST_VALUE) == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, VARIABLE_TEST_VALUE, testEntityType::TEST_VARIABLE, testRelationshipType::TEST_MODIFIES));
//     REQUIRE(hashkey == getTestHash(ONE_STRING_TEST_VALUE, testEntityType::TEST_STATEMENT, VARIABLE_TEST_VALUE, testEntityType::TEST_VARIABLE, testRelationshipType::TEST_MODIFIES));
//     delete relationship;
//     delete assignStatement;
//     delete variable;
// }

// TEST_CASE("Caller Calls Callee Procedure Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     Procedure *caller = new Procedure(new std::string(PROCEDURE_TEST_VALUE));
//     Procedure *callee = new Procedure(new std::string(PROCEDURE_TEST_VALUE_2));
//     Relationship *relationship = new CallsRelationship(caller, callee);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_CALLS, (EntityType)testEntityType::TEST_PROCEDURE, &PROCEDURE_TEST_VALUE, (EntityType)testEntityType::TEST_PROCEDURE, &PROCEDURE_TEST_VALUE_2) == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, PROCEDURE_TEST_VALUE_2, testEntityType::TEST_PROCEDURE, testRelationshipType::TEST_CALLS));
//     REQUIRE(hashkey == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, PROCEDURE_TEST_VALUE_2, testEntityType::TEST_PROCEDURE, testRelationshipType::TEST_CALLS));
//     delete relationship;
//     delete caller;
//     delete callee;
// }

// TEST_CASE("Caller CallsStar Callee Procedure Relationship Literal Hashkey Generator") {
//     RelationshipLiteralHashkeyGenerator relationshipLiteralHashkeyGenerator;
//     Procedure *caller = new Procedure(new std::string(PROCEDURE_TEST_VALUE));
//     Procedure *callee = new Procedure(new std::string(PROCEDURE_TEST_VALUE_2));
//     Relationship *relationship = new CallsStarRelationship(caller, callee);

//     std::string hashkey = relationshipLiteralHashkeyGenerator.getHashKey(relationship);

//     REQUIRE(relationshipLiteralHashkeyGenerator.getHashKey((RelationshipType)testRelationshipType::TEST_CALLS_STAR, (EntityType)testEntityType::TEST_PROCEDURE, &PROCEDURE_TEST_VALUE, (EntityType)testEntityType::TEST_PROCEDURE, &PROCEDURE_TEST_VALUE_2) == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, PROCEDURE_TEST_VALUE_2, testEntityType::TEST_PROCEDURE, testRelationshipType::TEST_CALLS_STAR));
//     REQUIRE(hashkey == getTestHash(PROCEDURE_TEST_VALUE, testEntityType::TEST_PROCEDURE, PROCEDURE_TEST_VALUE_2, testEntityType::TEST_PROCEDURE, testRelationshipType::TEST_CALLS_STAR));
//     delete relationship;
//     delete caller;
//     delete callee;
// }
