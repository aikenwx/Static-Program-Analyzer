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
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBtestHelpers.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"


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
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship));

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
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2));

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
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                               ASSIGN_STATEMENT,
                                                                                               VARIABLE);

    REQUIRE(relationships2->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship3));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(1), relationship4));


    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete readStatement3;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationship4;
    delete relationships;
    delete relationships2;
}

TEST_CASE("Retrieve Statement follow Statement") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement2, printStatement);
    Relationship *relationship3 = new FollowsRelationship(printStatement, printStatement2);
    Relationship *relationship4 = new FollowsRelationship(printStatement2, readStatement);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (Relationship *relationship: *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete printStatement;
    delete printStatement2;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationship4;
    delete relationships;
}

TEST_CASE("Retrieve Statement Follows Statement") {

        RelationshipManager *relationshipManager = new RelationshipManager();
        ReadStatement *readStatement = new ReadStatement(1);
        ReadStatement *readStatement2 = new ReadStatement(2);
        PrintStatement *printStatement = new PrintStatement(3);
        PrintStatement *printStatement2 = new PrintStatement(4);

        Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
        Relationship *relationship2 = new FollowsRelationship(readStatement2, printStatement);
        Relationship *relationship3 = new FollowsRelationship(printStatement, printStatement2);
        Relationship *relationship4 = new FollowsRelationship(printStatement2, readStatement);

        relationshipManager->storeRelationship(relationship);
        relationshipManager->storeRelationship(relationship2);
        relationshipManager->storeRelationship(relationship3);
        relationshipManager->storeRelationship(relationship4);

        std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                  STATEMENT);

        REQUIRE(relationships->size() == 4);
        for (Relationship *relationship: *relationships) {
            REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
            REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
        }

        std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                READ_STATEMENT);

        REQUIRE(relationships2->size() == 1);
        REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship));

        std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, PRINT_STATEMENT,
                                                                                                PRINT_STATEMENT);

        REQUIRE(relationships3->size() == 1);
        REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship3));

        delete relationshipManager;
        delete readStatement;
        delete readStatement2;
        delete printStatement;
        delete printStatement2;
        delete relationship;
        delete relationship2;
        delete relationship3;
        delete relationship4;
        delete relationships;
        delete relationships2;
        delete relationships3;
}


TEST_CASE("Retrieve Statement Follows Statement2") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);


    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, whileStatement);
    FollowsRelationship *followsRelationship2 = new FollowsRelationship(whileStatement, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 2);
    for (Relationship *relationship: *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, IF_STATEMENT,
                                                                                              WHILE_STATEMENT);

    REQUIRE(relationships2->size() == 1);

    std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, WHILE_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships3->size() == 1);

    delete relationshipManager;
    delete ifStatement;
    delete whileStatement;
    delete readStatement;
    delete followsRelationship;
    delete followsRelationship2;
    delete relationships;
    delete relationships2;
    delete relationships3;
}

TEST_CASE("Retrieve While Parent While") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    WhileStatement *whileStatement3 = new WhileStatement(3);
    WhileStatement *whileStatement4 = new WhileStatement(4);

    Relationship *relationship = new ParentRelationship(whileStatement, whileStatement2);
    Relationship *relationship2 = new ParentRelationship(whileStatement2, whileStatement3);
    Relationship *relationship3 = new ParentRelationship(whileStatement3, whileStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, WHILE_STATEMENT,
                                                                                              WHILE_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2));

    delete relationshipManager;
    delete whileStatement;
    delete whileStatement2;
    delete whileStatement3;
    delete whileStatement4;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationships;
}

TEST_CASE("Retrieve Call Follows Calls") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    CallStatement *callStatement = new CallStatement(1);
    CallStatement *callStatement2 = new CallStatement(2);
    CallStatement *callStatement3 = new CallStatement(3);
    CallStatement *callStatement4 = new CallStatement(4);

    Relationship *relationship = new FollowsRelationship(callStatement, callStatement2);
    Relationship *relationship2 = new FollowsRelationship(callStatement2, callStatement3);
    Relationship *relationship3 = new FollowsRelationship(callStatement3, callStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                              CALL_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                               STATEMENT);

    std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                              CALL_STATEMENT);

    REQUIRE(relationships3->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(1), relationship2));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(2), relationship3));


    delete relationshipManager;
    delete callStatement;
    delete callStatement2;
    delete callStatement3;
    delete callStatement4;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationships;
    delete relationships3;
}


TEST_CASE("Retrieve Statement Parent Assignment") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);

    AssignStatement *assignStatement = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);
    AssignStatement *assignStatement3 = new AssignStatement(3);
    AssignStatement *assignStatement4 = new AssignStatement(4);

    Relationship *relationship = new ParentRelationship(readStatement, assignStatement);
    Relationship *relationship2 = new ParentRelationship(readStatement2, assignStatement2);
    Relationship *relationship3 = new ParentRelationship(printStatement, printStatement2);
    Relationship *relationship4 = new ParentRelationship(printStatement2, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, STATEMENT,
                                                                                              ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 3);
    for (Relationship *relationship: *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(relationship->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    }

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete printStatement;
    delete printStatement2;
    delete assignStatement;
    delete assignStatement2;
    delete assignStatement3;
    delete assignStatement4;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationship4;
    delete relationships;
}

TEST_CASE("Retrieve Assign Parent Statement") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);

    AssignStatement *assignStatement = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);
    AssignStatement *assignStatement3 = new AssignStatement(3);
    AssignStatement *assignStatement4 = new AssignStatement(4);

    Relationship *relationship = new ParentRelationship(readStatement, assignStatement);
    Relationship *relationship2 = new ParentRelationship(readStatement2, assignStatement2);
    Relationship *relationship3 = new ParentRelationship(assignStatement2, printStatement2);
    Relationship *relationship4 = new ParentRelationship(printStatement2, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, ASSIGN_STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship3));

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete printStatement;
    delete printStatement2;
    delete assignStatement;
    delete assignStatement2;
    delete assignStatement3;
    delete assignStatement4;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationship4;
    delete relationships;


}

TEST_CASE("Retrieve Statement Follows Statement from empty RelationshipManager") {

    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement2, printStatement);
    Relationship *relationship3 = new FollowsRelationship(printStatement, printStatement2);
    Relationship *relationship4 = new FollowsRelationship(printStatement2, readStatement);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
    delete readStatement;
    delete readStatement2;
    delete printStatement;
    delete printStatement2;
    delete relationship;
    delete relationship2;
    delete relationship3;
    delete relationship4;
    delete relationships;
}
