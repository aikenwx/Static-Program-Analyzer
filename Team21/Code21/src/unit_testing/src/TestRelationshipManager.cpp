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

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    delete relationshipManager;
}

TEST_CASE("RelationshipManager retrieves relationship") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                             READ_STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship));

    delete relationshipManager;
}

TEST_CASE("RelationshipManager retrieves multitple relationships") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    ReadStatement *readStatement3 = new ReadStatement(3);
    ReadStatement *readStatement4 = new ReadStatement(4);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement3, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                             READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1).get(), relationship2));

    delete relationshipManager;
}

TEST_CASE("RelationshipManager returns empty vector if no entries") {
    RelationshipManager *relationshipManager = new RelationshipManager();

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                             READ_STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
}

TEST_CASE("RelationshipManager returns empty vector if no entries of type") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);

    relationshipManager->storeRelationship(relationship);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
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

    Variable *variable = new Variable("a");
    Variable *variable2 = new Variable("b");

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement3, readStatement4);
    Relationship *relationship3 = new ModifiesRelationship(assignStatement, variable);
    Relationship *relationship4 = new ModifiesRelationship(assignStatement2, variable2);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                             READ_STATEMENT);

    REQUIRE(relationships->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1).get(), relationship2));

    std::vector<std::shared_ptr<Relationship>> *relationships2 = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                                              ASSIGN_STATEMENT,
                                                                                                              VARIABLE);

    REQUIRE(relationships2->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0).get(), relationship3));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(1).get(), relationship4));

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

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement3, printStatement);
    Relationship *relationship3 = new FollowsRelationship(printStatement2, printStatement3);
    Relationship *relationship4 = new FollowsRelationship(printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                             STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (std::shared_ptr<Relationship> relationship : *relationships) {
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

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement3, printStatement);
    Relationship *relationship3 = new FollowsRelationship(printStatement2, printStatement3);
    Relationship *relationship4 = new FollowsRelationship(printStatement4, readStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                             STATEMENT);

    REQUIRE(relationships->size() == 4);
    for (std::shared_ptr<Relationship> relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    std::vector<std::shared_ptr<Relationship>> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, READ_STATEMENT,
                                                                                                              READ_STATEMENT);

    REQUIRE(relationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0).get(), relationship));

    std::vector<std::shared_ptr<Relationship>> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, PRINT_STATEMENT,
                                                                                                              PRINT_STATEMENT);

    REQUIRE(relationships3->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0).get(), relationship3));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Follows Statement2") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, whileStatement);
    FollowsRelationship *followsRelationship2 = new FollowsRelationship(whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                             STATEMENT);

    REQUIRE(relationships->size() == 2);
    for (std::shared_ptr<Relationship> relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(Entity::isStatementType(relationship->getRightHandEntity()->getEntityType()));
    }

    std::vector<std::shared_ptr<Relationship>> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, IF_STATEMENT,
                                                                                                              WHILE_STATEMENT);

    REQUIRE(relationships2->size() == 1);

    std::vector<std::shared_ptr<Relationship>> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, WHILE_STATEMENT,
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

    Relationship *relationship = new ParentRelationship(whileStatement, whileStatement2);
    Relationship *relationship2 = new ParentRelationship(whileStatement3, whileStatement4);
    Relationship *relationship3 = new ParentRelationship(whileStatement5, whileStatement6);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, WHILE_STATEMENT,
                                                                                                             WHILE_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1).get(), relationship2));

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

    Relationship *relationship = new FollowsRelationship(callStatement, callStatement2);
    Relationship *relationship2 = new FollowsRelationship(callStatement3, callStatement4);
    Relationship *relationship3 = new FollowsRelationship(callStatement5, callStatement6);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                                             CALL_STATEMENT);

    REQUIRE(relationships->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1).get(), relationship2));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2).get(), relationship3));

    std::vector<std::shared_ptr<Relationship>> *relationships2 = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                              STATEMENT);

    std::vector<std::shared_ptr<Relationship>> *relationships3 = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                                              CALL_STATEMENT);

    REQUIRE(relationships3->size() == 3);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0).get(), relationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(1).get(), relationship2));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(2).get(), relationship3));

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
    AssignStatement *assignStatement3 = new AssignStatement(8);
    AssignStatement *assignStatement4 = new AssignStatement(9);

    Relationship *relationship = new ParentRelationship(readStatement, assignStatement);
    Relationship *relationship2 = new ParentRelationship(readStatement2, assignStatement2);
    Relationship *relationship3 = new ParentRelationship(printStatement, printStatement2);
    Relationship *relationship4 = new ParentRelationship(printStatement3, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, STATEMENT,
                                                                                                             ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 3);
    for (std::shared_ptr<Relationship> relationship : *relationships) {
        REQUIRE(Entity::isStatementType(relationship->getLeftHandEntity()->getEntityType()));
        REQUIRE(relationship->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    }

    delete relationshipManager;
}

TEST_CASE("Retrieve Assign Parent Statement") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);
    PrintStatement *printStatement3 = new PrintStatement(5);

    AssignStatement *assignStatement = new AssignStatement(6);
    AssignStatement *assignStatement2 = new AssignStatement(7);
    AssignStatement *assignStatement3 = new AssignStatement(8);
    AssignStatement *assignStatement4 = new AssignStatement(9);

    Relationship *relationship = new ParentRelationship(readStatement, assignStatement);
    Relationship *relationship2 = new ParentRelationship(readStatement2, assignStatement2);
    Relationship *relationship3 = new ParentRelationship(assignStatement3, printStatement2);
    Relationship *relationship4 = new ParentRelationship(printStatement3, assignStatement4);

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, ASSIGN_STATEMENT,
                                                                                                             STATEMENT);

    REQUIRE(relationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0).get(), relationship3));

    delete relationshipManager;
}

TEST_CASE("Retrieve Statement Follows Statement from empty RelationshipManager") {
    RelationshipManager *relationshipManager = new RelationshipManager();
    ReadStatement *readStatement = new ReadStatement(1);
    ReadStatement *readStatement2 = new ReadStatement(2);
    PrintStatement *printStatement = new PrintStatement(3);
    PrintStatement *printStatement2 = new PrintStatement(4);
    PrintStatement *printStatement3 = new PrintStatement(5);
    PrintStatement *printStatement4 = new PrintStatement(6);
    ReadStatement *readStatement3 = new ReadStatement(7);

    Relationship *relationship = new FollowsRelationship(readStatement, readStatement2);
    Relationship *relationship2 = new FollowsRelationship(readStatement3, printStatement);
    Relationship *relationship3 = new FollowsRelationship(printStatement2, printStatement3);
    Relationship *relationship4 = new FollowsRelationship(printStatement4, readStatement);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, STATEMENT,
                                                                                                             STATEMENT);

    REQUIRE(relationships->size() == 0);

    delete relationshipManager;
}
