//
// Created by Aiken Wong on 4/2/23.
//

#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKB/RelationshipLiteralHashkeyGenerator.h"
#include "PKBtestHelpers.h"
#include "catch.hpp"

TEST_CASE("RelationshipManager instantiates") {
    RelationshipManager *relationshipManager = new RelationshipManager();

    delete relationshipManager;
}

TEST_CASE("RelationshipManager stores relationship") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    delete relationshipManager;
}

TEST_CASE("RelationshipManager retrieves relationship") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));

    delete relationshipManager;
}

TEST_CASE("RelationshipManager retrieves multitple relationships") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);
    ReadStatement *readStatement4 = new ReadStatement(4);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));

    delete relationshipManager;
}

TEST_CASE("RelationshipManager returns empty vector if no entries") {
    RelationshipManager *relationshipManager = new RelationshipManager();

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
}

TEST_CASE("RelationshipManager returns empty vector if no entries of type") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
}

TEST_CASE("RelationshipManager can retrieve mulitple relationships of multiple types") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);
    ReadStatement *readStatement4 = new ReadStatement(4);

    AssignStatement *assignStatement = new AssignStatement(1);
    AssignStatement *assignStatement2 = new AssignStatement(2);

    Variable *variable = new Variable(new std::string("a"));
    Variable *variable2 = new Variable(new std::string("b"));

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, readStatement4);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ModifiesRelationship>(assignStatement, variable);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<ModifiesRelationship>(assignStatement2, variable2);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                              READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                               ASSIGN_STATEMENT,
                                                                                               VARIABLE);

    REQUIRE(relationships2->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship3.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(1), relationship4.get()));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement follow Statement") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);
    ReadStatement *readStatement4 = new ReadStatement(4);

    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);
    PrintStatement *printStatement3 = new PrintStatement(5);
    PrintStatement *printStatement4 = new PrintStatement(6);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, printStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(printStatement2, printStatement3);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (Relationship *relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Follows Statement") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);
    ReadStatement *readStatement4 = new ReadStatement(4);

    PrintStatement *printStatement = new PrintStatement(5);
    PrintStatement *printStatement2 = new PrintStatement(6);
    PrintStatement *printStatement3 = new PrintStatement(7);
    PrintStatement *printStatement4 = new PrintStatement(8);

    std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, printStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(printStatement2, printStatement3);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (Relationship *relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                               READ_STATEMENT);

    REQUIRE(relationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship.get()));

    std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, PRINT_STATEMENT,
                                                                                               PRINT_STATEMENT);

    REQUIRE(relationships3->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship3.get()));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Follows Statement2") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship>(ifStatement, whileStatement);
    std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_shared<FollowsRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 2);
    for (Relationship *relationship : *relationships) {
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
}

TEST_CASE("Retrieve While Parent While") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    WhileStatement *whileStatement3 = new WhileStatement(3);
    WhileStatement *whileStatement4 = new WhileStatement(4);
    WhileStatement *whileStatement5 = new WhileStatement(5);
    WhileStatement *whileStatement6 = new WhileStatement(6);

    std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(whileStatement3, whileStatement4);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(whileStatement5, whileStatement6);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, WHILE_STATEMENT,
                                                                                              WHILE_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));

    delete relationshipManager;
}

TEST_CASE("Retrieve Call Follows Calls") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    CallStatement *callStatement = new CallStatement(1);
    CallStatement *callStatement2 = new CallStatement(2);
    CallStatement *callStatement3 = new CallStatement(3);
    CallStatement *callStatement4 = new CallStatement(4);
    CallStatement *callStatement5 = new CallStatement(5);
    CallStatement *callStatement6 = new CallStatement(6);

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsRelationship>(callStatement, callStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(callStatement3, callStatement4);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(callStatement5, callStatement6);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                              CALL_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                               STATEMENT);

    std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                               CALL_STATEMENT);

    REQUIRE(relationships3->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(1), relationship2.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(2), relationship3.get()));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Parent Assignment") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);
    PrintStatement *printStatement3 = new PrintStatement(5);

    AssignStatement *assignStatement = new AssignStatement(6);
    AssignStatement *assignStatement2 = new AssignStatement(7);
    AssignStatement *assignStatement4 = new AssignStatement(9);

    std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(readStatement, assignStatement);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(readStatement2, assignStatement2);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(printStatement, printStatement2);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentRelationship>(printStatement3, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, STATEMENT,
                                                                                              ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 3);
    for (Relationship *relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(relationship->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    }

    delete relationshipManager;
}

TEST_CASE("Retrieve Assign Parent Statement") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement2 = new PrintStatement(4);
    PrintStatement *printStatement3 = new PrintStatement(5);

    AssignStatement *assignStatement = new AssignStatement(6);
    AssignStatement *assignStatement2 = new AssignStatement(7);
    AssignStatement *assignStatement3 = new AssignStatement(8);
    AssignStatement *assignStatement4 = new AssignStatement(9);

    std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(readStatement, assignStatement);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(readStatement2, assignStatement2);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(assignStatement3, printStatement2);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentRelationship>(printStatement3, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, ASSIGN_STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship3.get()));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Follows Statement from empty RelationshipManager") {
    RelationshipManager *relationshipManager = new RelationshipManager();

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);
    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
}

TEST_CASE("Retrieve While FollowsStar While") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);
    WhileStatement *whileStatement3 = new WhileStatement(3);
    WhileStatement *whileStatement4 = new WhileStatement(4);
    WhileStatement *whileStatement5 = new WhileStatement(5);
    WhileStatement *whileStatement6 = new WhileStatement(6);

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement4);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsStarRelationship>(whileStatement5, whileStatement6);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS_STAR, WHILE_STATEMENT,
                                                                                              WHILE_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));

    delete relationshipManager;
    delete whileStatement;
    delete whileStatement2;
    delete whileStatement3;
    delete whileStatement4;
    delete whileStatement5;
    delete whileStatement6;
}

TEST_CASE("Retrieve While FollowsStar Statement") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    PrintStatement *printStatement = new PrintStatement(2);
    AssignStatement *assignStatement = new AssignStatement(3);
    WhileStatement *whileStatement2 = new WhileStatement(4);
    WhileStatement *whileStatement3 = new WhileStatement(5);
    IfStatement *ifStatement = new IfStatement(6);
    CallStatement *callStatement = new CallStatement(7);

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, printStatement);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement2, assignStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement2);
    std::shared_ptr<Relationship> relationship5 = std::make_shared<FollowsStarRelationship>(whileStatement3, ifStatement);
    std::shared_ptr<Relationship> relationship6 = std::make_shared<FollowsStarRelationship>(ifStatement, callStatement);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);
    relationshipManager->storeRelationship(relationship5);
    relationshipManager->storeRelationship(relationship6);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS_STAR, WHILE_STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 5);
    for (Relationship *relationship : *relationships) {
        bool isInRelationships = false;
        for (Relationship *relationship2 : *relationships) {
            if (PKBtestHelpers::relationshipEqualsRelationship(relationship, relationship2)) {
                isInRelationships = true;
                break;
            }
        }
        REQUIRE(isInRelationships);
    }

    delete relationshipManager;
    delete whileStatement;
    delete whileStatement2;
    delete whileStatement3;
    delete printStatement;
    delete assignStatement;
    delete ifStatement;
    delete callStatement;
}

TEST_CASE("Retrieve If ParentStar Call") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    IfStatement *ifStatement = new IfStatement(1);
    CallStatement *callStatement = new CallStatement(2);
    CallStatement *callStatement2 = new CallStatement(3);
    CallStatement *callStatement3 = new CallStatement(4);
    CallStatement *callStatement4 = new CallStatement(5);
    CallStatement *callStatement5 = new CallStatement(6);
    PrintStatement *printStatement = new PrintStatement(7);

    std::shared_ptr<Relationship> relationship = std::make_shared<ParentStarRelationship>(ifStatement, callStatement);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement2);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement3);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement4);
    std::shared_ptr<Relationship> relationship5 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement5);
    std::shared_ptr<Relationship> relationship6 = std::make_shared<ParentStarRelationship>(ifStatement, printStatement);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);
    relationshipManager->storeRelationship(relationship5);
    relationshipManager->storeRelationship(relationship6);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT_STAR, IF_STATEMENT,
                                                                                              CALL_STATEMENT);

    REQUIRE(relationships->size() == 5);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(3), relationship4.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(4), relationship5.get()));

    delete relationshipManager;
    delete ifStatement;
    delete callStatement;
    delete callStatement2;
    delete callStatement3;
    delete callStatement4;
    delete callStatement5;
    delete printStatement;
}

TEST_CASE("Retrieve Statement Follows Call") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    CallStatement *callStatement = new CallStatement(1);
    AssignStatement *assignStatement = new AssignStatement(2);
    AssignStatement *assignStatement2 = new AssignStatement(3);
    AssignStatement *assignStatement3 = new AssignStatement(4);
    AssignStatement *assignStatement4 = new AssignStatement(5);
    AssignStatement *assignStatement5 = new AssignStatement(6);
    PrintStatement *printStatement = new PrintStatement(7);

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsRelationship>(assignStatement, callStatement);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(assignStatement2, callStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(assignStatement3, callStatement);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(assignStatement4, callStatement);
    std::shared_ptr<Relationship> relationship5 = std::make_shared<FollowsRelationship>(assignStatement5, callStatement);
    std::shared_ptr<Relationship> relationship6 = std::make_shared<FollowsRelationship>(printStatement, assignStatement5);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);
    relationshipManager->storeRelationship(relationship5);
    relationshipManager->storeRelationship(relationship6);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              CALL_STATEMENT);

    REQUIRE(relationships->size() == 5);
    for (Relationship *relationship : *relationships) {
        bool isInRelationships = false;
        for (Relationship *relationship2 : *relationships) {
            if (PKBtestHelpers::relationshipEqualsRelationship(relationship, relationship2)) {
                isInRelationships = true;
                break;
            }
        }
        REQUIRE(isInRelationships);
    }

    delete relationshipManager;
    delete callStatement;
    delete assignStatement;
    delete assignStatement2;
    delete assignStatement3;
    delete assignStatement4;
    delete assignStatement5;
    delete printStatement;
}

TEST_CASE("Duplicate Relationship throws error") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);

    relationshipManager->storeRelationship(relationship);
    REQUIRE_THROWS(relationshipManager->storeRelationship(relationship2));

    delete relationshipManager;
    delete whileStatement;
    delete whileStatement2;
}

TEST_CASE("Can retrieve relationship by literal values") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    WhileStatement *whileStatement = new WhileStatement(1);
    WhileStatement *whileStatement2 = new WhileStatement(2);

    AssignStatement *assignStatement = new AssignStatement(3);
    AssignStatement *assignStatement2 = new AssignStatement(4);
    CallStatement *callStatement = new CallStatement(5);
    ReadStatement *readStatement = new ReadStatement(6);
    PrintStatement *printStatement = new PrintStatement(7);
    IfStatement *ifStatement = new IfStatement(8);
    Variable *variable = new Variable(new std::string("x"));
    Variable *variable2 = new Variable(new std::string("y"));
    Procedure *procedure = new Procedure(new std::string("main"));

    RelationshipLiteralHashkeyGenerator hashkeyGenerator;
    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(whileStatement, assignStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentStarRelationship>(assignStatement, assignStatement2);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentRelationship>(assignStatement, callStatement);
    std::shared_ptr<Relationship> relationship5 = std::make_shared<UsesRelationship>(callStatement, variable);
    std::shared_ptr<Relationship> relationship6 = std::make_shared<ModifiesRelationship>(callStatement, variable2);
    std::shared_ptr<Relationship> relationship7 = std::make_shared<ModifiesRelationship>(readStatement, variable2);
    std::shared_ptr<Relationship> relationship9 = std::make_shared<UsesRelationship>(procedure, variable);
    std::shared_ptr<Relationship> relationship10 = std::make_shared<ModifiesRelationship>(ifStatement, variable2);
    std::shared_ptr<Relationship> relationship11 = std::make_shared<UsesRelationship>(printStatement, variable2);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);
    relationshipManager->storeRelationship(relationship5);
    relationshipManager->storeRelationship(relationship6);
    relationshipManager->storeRelationship(relationship7);
    relationshipManager->storeRelationship(relationship9);
    relationshipManager->storeRelationship(relationship10);
    relationshipManager->storeRelationship(relationship11);


    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship2.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship2.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship3.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship3.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship4.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship4.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship5.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship5.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship6.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship6.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship7.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship7.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship9.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship9.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship10.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship10.get()))));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationship11.get(), relationshipManager->getRelationshipByLiterals(hashkeyGenerator.getHashKey(relationship11.get()))));

    delete relationshipManager;
    delete whileStatement;
    delete whileStatement2;
    delete assignStatement;
    delete assignStatement2;
    delete callStatement;
    delete readStatement;
    delete printStatement;
    delete ifStatement;
    delete variable;
    delete variable2;
    delete procedure;
}