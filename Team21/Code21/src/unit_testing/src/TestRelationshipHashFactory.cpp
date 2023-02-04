//
// Created by Aiken Wong on 4/2/23.
//
#include <stdexcept>

#include "catch.hpp"

#include <string>

#include "PKB/RelationshipHashFactory.h"

#include "PKBStorageClasses/EntityClasses/Entity.h"

#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"


TEST_CASE("Assignment statement parent of read statement") {
    RelationshipHashFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::PARENT;
    EntityType leftHandEntityType = EntityType::WHILE_STATEMENT;
    EntityType rightHandEntityType = EntityType::ASSIGN_STATEMENT;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "parent_While_Assign");
}

TEST_CASE("Assignment modifies variable") {
    RelationshipHashFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::MODIFIES;
    EntityType leftHandEntityType = EntityType::ASSIGN_STATEMENT;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "modifies_Assign_Variable");
}

TEST_CASE("Procedure uses variable") {
    RelationshipHashFactory relationshipHashFactory;
    RelationshipType relationshipType = RelationshipType::USES;
    EntityType leftHandEntityType = EntityType::PROCEDURE;
    EntityType rightHandEntityType = EntityType::VARIABLE;
    std::string hashKey = relationshipHashFactory.getHashKey(relationshipType, leftHandEntityType, rightHandEntityType);
    REQUIRE(hashKey == "uses_Procedure_Variable");
}

TEST_CASE("Apply hash factory on Modifies Relationship") {
    Variable *variable = new Variable("x");
    AssignStatement *assignStatement = new AssignStatement(1);
    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(assignStatement, variable);
    RelationshipHashFactory relationshipHashFactory;
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
    RelationshipHashFactory relationshipHashFactory;
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
    RelationshipHashFactory relationshipHashFactory;
    std::string hashKey = relationshipHashFactory.getHashKey(followsRelationship);
    REQUIRE(hashKey == "follows_Assign_While");

    delete followsRelationship;
    delete assignStatement;
    delete whileStatement;
}

