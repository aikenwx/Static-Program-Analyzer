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
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
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

    AssignStatement *assignStatement = new AssignStatement(1, new std::string("ab-"));
    AssignStatement *assignStatement2 = new AssignStatement(2, new std::string("ab-"));

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
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship> (readStatement3, printStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship> (printStatement2, printStatement3);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship> (printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (Relationship* relationship : *relationships) {
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
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship> (readStatement3, printStatement);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship> (printStatement2, printStatement3);
    std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship> (printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (Relationship* relationship : *relationships) {
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

    std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship> (ifStatement, whileStatement);
    std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_shared<FollowsRelationship> (whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                              STATEMENT);

    REQUIRE(relationships->size() == 2);
    for (Relationship* relationship : *relationships) {
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

    std::shared_ptr<Relationship> relationship = std::make_shared<FollowsRelationship> (callStatement, callStatement2);
    std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship> (callStatement3, callStatement4);
    std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship> (callStatement5, callStatement6);

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

    AssignStatement *assignStatement = new AssignStatement(6, new std::string("ab-"));
    AssignStatement *assignStatement2 = new AssignStatement(7, new std::string("ab-"));
    AssignStatement *assignStatement4 = new AssignStatement(9, new std::string("ab-"));

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
    for (Relationship* relationship : *relationships) {
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

    AssignStatement *assignStatement = new AssignStatement(6, new std::string("ab-"));
    AssignStatement *assignStatement2 = new AssignStatement(7, new std::string("ab-"));
    AssignStatement *assignStatement3 = new AssignStatement(8, new std::string("ab-"));
    AssignStatement *assignStatement4 = new AssignStatement(9, new std::string("ab-"));

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
