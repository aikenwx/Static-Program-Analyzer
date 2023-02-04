//
// Created by Aiken Wong on 4/2/23.
//


#include "catch.hpp"

#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"


bool relationshipEqualsRelationship(Relationship *relationship1, Relationship *relationship2) {
    return relationship1->getRelationshipType() == relationship2->getRelationshipType() &&
           relationship1->getLeftHandEntity()->equals(relationship2->getLeftHandEntity()) &&
           relationship1->getRightHandEntity()->equals(relationship2->getRightHandEntity());
}

TEST_CASE("RelationshipManager instantiates") {

    RelationshipManager *relationshipManager = new RelationshipManager();

    delete relationshipManager;
}

TEST_CASE("RelationshipManager stores relationship") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);


    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete relationship;
}

TEST_CASE("RelationshipManager retrieves relationship") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(relationshipEqualsRelationship(relationships->at(0), relationship));

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete relationship;
    delete relationships;
}

TEST_CASE("RelationshipManager retrieves multitple relationships") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement2, readStatement3);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(relationshipEqualsRelationship(relationships->at(1), relationship2));

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete readStatement3;
    delete relationship;
    delete relationship2;
    delete relationships;
}

TEST_CASE("RelationshipManager returns empty vector if no entries") {

    RelationshipManager *relationshipManager = new RelationshipManager();

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
    delete relationships;

}

TEST_CASE("RelationshipManager returns empty vector if no entries of type") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete relationship;
    delete relationships;

}

TEST_CASE("RelationshipManager can retrieve mulitple relationships of multiple types") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);

    AssignStatement *assignStatement = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);

    Variable *variable = new Variable("a");
    Variable *variable2 = new Variable("b");

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement2, readStatement3);
    Relationship *relationship3 = new ModifiesRelationship(assignStatement, variable);
    Relationship *relationship4 = new ModifiesRelationship(assignStatement2, variable2);


    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(relationshipEqualsRelationship(relationships->at(1), relationship2));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                               ASSIGN_STATEMENT,
                                                                                               VARIABLE);

    REQUIRE(relationships2->size() == 2);
    REQUIRE(relationshipEqualsRelationship(relationships2->at(0), relationship3));
    REQUIRE(relationshipEqualsRelationship(relationships2->at(1), relationship4));


    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete readStatement3;
    delete relationship;
    delete relationship2;
    delete relationships;
    delete relationships2;
}
