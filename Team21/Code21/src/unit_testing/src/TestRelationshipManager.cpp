// //
// // Created by Aiken Wong on 4/2/23.
// //

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>

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
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBtestHelpers.h"

TEST_CASE("RelationshipManager instantiates") {
    RelationshipManager relationshipManager = RelationshipManager();
}

TEST_CASE("RelationshipManager stores relationship") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();

    auto readStatement = std::make_shared<ReadStatement>(1);
    auto readStatement2 = std::make_shared<ReadStatement>(2);

    relationshipManager->storeRelationship(std::make_shared<FollowsRelationship>(readStatement.get(), readStatement2.get()));
}

TEST_CASE("RelationshipManager retrieves relationship") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();
    auto readStatement = std::make_shared<ReadStatement>(1);
    auto readStatement2 = std::make_shared<ReadStatement>(2);


    auto relationship = std::make_shared<FollowsRelationship>(readStatement.get(), readStatement2.get());
    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
        FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
        ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->equals(relationship.get()));
}

TEST_CASE("RelationshipManager retrieves multitple relationships") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();
    auto readStatement = std::make_shared<ReadStatement>(1);
    auto readStatement2 = std::make_shared<ReadStatement>(2);
    auto readStatement3 = std::make_shared<ReadStatement>(3);
    auto readStatement4 = std::make_shared<ReadStatement>(4);

    auto relationship = std::make_shared<FollowsRelationship>(readStatement.get(), readStatement2.get());

    auto relationship2 = std::make_shared<FollowsRelationship>(readStatement3.get(), readStatement4.get());

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
        FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
        ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);
    REQUIRE(relationships->at(0)->equals(relationship.get()));
    REQUIRE(relationships->at(1)->equals(relationship2.get()));
}

TEST_CASE("RelationshipManager returns empty vector if no entries") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
        FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
        ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->empty());

}

TEST_CASE("RelationshipManager returns empty vector if no entries of type") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();
    auto readStatement = std::make_shared<ReadStatement>(1);
    auto readStatement2 = std::make_shared<ReadStatement>(2);

    auto relationship = std::make_shared<FollowsRelationship>(readStatement.get(), readStatement2.get());

    
    relationshipManager->storeRelationship(relationship);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
        FollowsStarRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
        ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->empty());

}

TEST_CASE("RelationshipManager can retrieve mulitple relationships of multiple types") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();
    auto readStatement = std::make_shared<ReadStatement>(1);
    auto readStatement2 = std::make_shared<ReadStatement>(2);
    auto readStatement3 = std::make_shared<ReadStatement>(3);
    auto readStatement4 = std::make_shared<ReadStatement>(4);
    auto assignStatement = std::make_shared<AssignStatement>(5);
    auto assignStatement2 = std::make_shared<AssignStatement>(6);

    auto aString = std::make_shared<std::string>("a");
    auto bString = std::make_shared<std::string>("b");
    auto variable = std::make_shared<Variable>(aString);
    auto variable2 = std::make_shared<Variable>(bString);

    auto relationship = std::make_shared<FollowsRelationship>(readStatement.get(), readStatement2.get());
    auto relationship2 = std::make_shared<FollowsRelationship>(readStatement3.get(), readStatement4.get());
    auto relationship3 = std::make_shared<ModifiesRelationship>(assignStatement.get(), variable.get());
    auto relationship4 = std::make_shared<ModifiesRelationship>(assignStatement2.get(), variable2.get());

    relationshipManager->storeRelationship(relationship);
    relationshipManager->storeRelationship(relationship2);
    relationshipManager->storeRelationship(relationship3);
    relationshipManager->storeRelationship(relationship4);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
        FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
        ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(
        ModifiesRelationship::getRelationshipTypeStatic(), AssignStatement::getEntityTypeStatic(),
        Variable::getEntityTypeStatic());

    REQUIRE(relationships2->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship3.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(1), relationship4.get()));

}

// TEST_CASE("Retrieve Statement follow Statement") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     ReadStatement *readStatement3 = new ReadStatement(3);
//     ReadStatement *readStatement4 = new ReadStatement(4);

//     PrintStatement *printStatement = new PrintStatement(3);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);
//     PrintStatement *printStatement4 = new PrintStatement(6);

//     std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, printStatement);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(printStatement2, printStatement3);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(printStatement4, readStatement4);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               STATEMENT);

//     REQUIRE(relationships->size() == 4);
//     for (Relationship *relationship : *relationships) {
//         REQUIRE(Statement::isStatement(relationship->getLeftHandEntity()));
//         REQUIRE(Statement::isStatement(relationship->getRightHandEntity()));
//     }

// //     delete readStatement;
//     delete readStatement2;
//     delete readStatement3;
//     delete readStatement4;
//     delete printStatement;
//     delete printStatement2;
//     delete printStatement3;
//     delete printStatement4;
// }

// TEST_CASE("Retrieve Statement Follows Statement") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     ReadStatement *readStatement3 = new ReadStatement(3);
//     ReadStatement *readStatement4 = new ReadStatement(4);

//     PrintStatement *printStatement = new PrintStatement(5);
//     PrintStatement *printStatement2 = new PrintStatement(6);
//     PrintStatement *printStatement3 = new PrintStatement(7);
//     PrintStatement *printStatement4 = new PrintStatement(8);

//     std::shared_ptr<FollowsRelationship> relationship = std::make_shared<FollowsRelationship>(readStatement, readStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(readStatement3, printStatement);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(printStatement2, printStatement3);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(printStatement4, readStatement4);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               STATEMENT);

//     REQUIRE(relationships->size() == 4);
//     for (Relationship *relationship : *relationships) {
//         REQUIRE(Statement::isStatement(relationship->getLeftHandEntity()));
//         REQUIRE(Statement::isStatement(relationship->getRightHandEntity()));
//     }

//     std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, READ_STATEMENT,
//                                                                                                READ_STATEMENT);

//     REQUIRE(relationships2->size() == 1);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships2->at(0), relationship.get()));

//     std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, PRINT_STATEMENT,
//                                                                                                PRINT_STATEMENT);

//     REQUIRE(relationships3->size() == 1);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship3.get()));

// //     delete readStatement;
//     delete readStatement2;
//     delete readStatement3;
//     delete readStatement4;
//     delete printStatement;
//     delete printStatement2;
//     delete printStatement3;
//     delete printStatement4;
// }

// TEST_CASE("Retrieve Statement Follows Statement2") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     IfStatement *ifStatement = new IfStatement(1);
//     WhileStatement *whileStatement = new WhileStatement(2);
//     WhileStatement *whileStatement2 = new WhileStatement(3);
//     ReadStatement *readStatement = new ReadStatement(4);

//     std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship>(ifStatement, whileStatement);
//     std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_shared<FollowsRelationship>(whileStatement2, readStatement);

//     relationshipManager->storeRelationship(followsRelationship);
//     relationshipManager->storeRelationship(followsRelationship2);

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               STATEMENT);

//     REQUIRE(relationships->size() == 2);
//     for (Relationship *relationship : *relationships) {
//         REQUIRE(Statement::isStatement(relationship->getLeftHandEntity()));
//         REQUIRE(Statement::isStatement(relationship->getRightHandEntity()));
//     }

//     std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, IF_STATEMENT,
//                                                                                                WHILE_STATEMENT);

//     REQUIRE(relationships2->size() == 1);

//     std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, WHILE_STATEMENT,
//                                                                                                READ_STATEMENT);

//     REQUIRE(relationships3->size() == 1);

// //     delete ifStatement;
//     delete whileStatement;
//     delete whileStatement2;
//     delete readStatement;
// }

// TEST_CASE("Retrieve While Parent While") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);
//     WhileStatement *whileStatement3 = new WhileStatement(3);
//     WhileStatement *whileStatement4 = new WhileStatement(4);
//     WhileStatement *whileStatement5 = new WhileStatement(5);
//     WhileStatement *whileStatement6 = new WhileStatement(6);

//     std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(whileStatement, whileStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(whileStatement3, whileStatement4);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(whileStatement5, whileStatement6);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(PARENT, WHILE_STATEMENT,
//                                                                                               WHILE_STATEMENT);

//     REQUIRE(relationships->size() == 3);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));

// //     delete whileStatement;
//     delete whileStatement2;
//     delete whileStatement3;
//     delete whileStatement4;
//     delete whileStatement5;
//     delete whileStatement6;
// }

// TEST_CASE("Retrieve Call Follows Calls") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     CallStatement *callStatement = new CallStatement(1);
//     CallStatement *callStatement2 = new CallStatement(2);
//     CallStatement *callStatement3 = new CallStatement(3);
//     CallStatement *callStatement4 = new CallStatement(4);
//     CallStatement *callStatement5 = new CallStatement(5);
//     CallStatement *callStatement6 = new CallStatement(6);

//     std::shared_ptr<Relationship> relationship = std::make_shared<FollowsRelationship>(callStatement, callStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(callStatement3, callStatement4);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(callStatement5, callStatement6);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, CALL_STATEMENT,
//                                                                                               CALL_STATEMENT);

//     REQUIRE(relationships->size() == 3);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));

//     std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                                STATEMENT);

//     std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, CALL_STATEMENT,
//                                                                                                CALL_STATEMENT);

//     REQUIRE(relationships3->size() == 3);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(0), relationship.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(1), relationship2.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships3->at(2), relationship3.get()));

// //     delete callStatement;
//     delete callStatement2;
//     delete callStatement3;
//     delete callStatement4;
//     delete callStatement5;
//     delete callStatement6;
// }

// TEST_CASE("Retrieve Statement Parent Assignment") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     PrintStatement *printStatement = new PrintStatement(3);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);

//     AssignStatement *assignStatement = new AssignStatement(6);
//     AssignStatement *assignStatement2 = new AssignStatement(7);
//     AssignStatement *assignStatement4 = new AssignStatement(9);

//     std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(readStatement, assignStatement);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(readStatement2, assignStatement2);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(printStatement, printStatement2);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentRelationship>(printStatement3, assignStatement4);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(PARENT, STATEMENT,
//                                                                                               ASSIGN_STATEMENT);

//     REQUIRE(relationships->size() == 3);
//     for (Relationship *relationship : *relationships) {
//         REQUIRE(Statement::isStatement(relationship->getLeftHandEntity()));
//         REQUIRE(relationship->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
//     }

// //     delete readStatement;
//     delete readStatement2;
//     delete printStatement;
//     delete printStatement2;
//     delete printStatement3;
//     delete assignStatement;
//     delete assignStatement2;
//     delete assignStatement4;
// }

// TEST_CASE("Retrieve Assign Parent Statement") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);

//     AssignStatement *assignStatement = new AssignStatement(6);
//     AssignStatement *assignStatement2 = new AssignStatement(7);
//     AssignStatement *assignStatement3 = new AssignStatement(8);
//     AssignStatement *assignStatement4 = new AssignStatement(9);

//     std::shared_ptr<Relationship> relationship = std::make_shared<ParentRelationship>(readStatement, assignStatement);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentRelationship>(readStatement2, assignStatement2);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentRelationship>(assignStatement3, printStatement2);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentRelationship>(printStatement3, assignStatement4);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(PARENT, ASSIGN_STATEMENT,
//                                                                                               STATEMENT);

//     REQUIRE(relationships->size() == 1);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship3.get()));

// //     delete readStatement;
//     delete readStatement2;
//     delete printStatement2;
//     delete printStatement3;
//     delete assignStatement;
//     delete assignStatement2;
//     delete assignStatement3;
//     delete assignStatement4;
// }

// TEST_CASE("Retrieve Statement Follows Statement from empty RelationshipManager") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               STATEMENT);
//     REQUIRE(relationships->size() == 0);

// // }

// TEST_CASE("Retrieve While FollowsStar While") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);
//     WhileStatement *whileStatement3 = new WhileStatement(3);
//     WhileStatement *whileStatement4 = new WhileStatement(4);
//     WhileStatement *whileStatement5 = new WhileStatement(5);
//     WhileStatement *whileStatement6 = new WhileStatement(6);

//     std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement4);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsStarRelationship>(whileStatement5, whileStatement6);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS_STAR, WHILE_STATEMENT,
//                                                                                               WHILE_STATEMENT);

//     REQUIRE(relationships->size() == 3);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));

// //     delete whileStatement;
//     delete whileStatement2;
//     delete whileStatement3;
//     delete whileStatement4;
//     delete whileStatement5;
//     delete whileStatement6;
// }

// TEST_CASE("Retrieve While FollowsStar Statement") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     PrintStatement *printStatement = new PrintStatement(2);
//     AssignStatement *assignStatement = new AssignStatement(3);
//     WhileStatement *whileStatement2 = new WhileStatement(4);
//     WhileStatement *whileStatement3 = new WhileStatement(5);
//     IfStatement *ifStatement = new IfStatement(6);
//     CallStatement *callStatement = new CallStatement(7);

//     std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, printStatement);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement2, assignStatement);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsStarRelationship>(whileStatement3, whileStatement2);
//     std::shared_ptr<Relationship> relationship5 = std::make_shared<FollowsStarRelationship>(whileStatement3, ifStatement);
//     std::shared_ptr<Relationship> relationship6 = std::make_shared<FollowsStarRelationship>(ifStatement, callStatement);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship5));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship6));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS_STAR, WHILE_STATEMENT,
//                                                                                               STATEMENT);

//     REQUIRE(relationships->size() == 5);
//     for (Relationship *relationship : *relationships) {
//         bool isInRelationships = false;
//         for (Relationship *relationship2 : *relationships) {
//             if (PKBtestHelpers::relationshipEqualsRelationship(relationship, relationship2)) {
//                 isInRelationships = true;
//                 break;
//             }
//         }
//         REQUIRE(isInRelationships);
//     }

// //     delete whileStatement;
//     delete whileStatement2;
//     delete whileStatement3;
//     delete printStatement;
//     delete assignStatement;
//     delete ifStatement;
//     delete callStatement;
// }

// TEST_CASE("Retrieve If ParentStar Call") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     IfStatement *ifStatement = new IfStatement(1);
//     CallStatement *callStatement = new CallStatement(2);
//     CallStatement *callStatement2 = new CallStatement(3);
//     CallStatement *callStatement3 = new CallStatement(4);
//     CallStatement *callStatement4 = new CallStatement(5);
//     CallStatement *callStatement5 = new CallStatement(6);
//     PrintStatement *printStatement = new PrintStatement(7);

//     std::shared_ptr<Relationship> relationship = std::make_shared<ParentStarRelationship>(ifStatement, callStatement);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement2);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement3);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement4);
//     std::shared_ptr<Relationship> relationship5 = std::make_shared<ParentStarRelationship>(ifStatement, callStatement5);
//     std::shared_ptr<Relationship> relationship6 = std::make_shared<ParentStarRelationship>(ifStatement, printStatement);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship5));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship6));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(PARENT_STAR, IF_STATEMENT,
//                                                                                               CALL_STATEMENT);

//     REQUIRE(relationships->size() == 5);
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), relationship.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), relationship2.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(2), relationship3.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(3), relationship4.get()));
//     REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(4), relationship5.get()));

// //     delete ifStatement;
//     delete callStatement;
//     delete callStatement2;
//     delete callStatement3;
//     delete callStatement4;
//     delete callStatement5;
//     delete printStatement;
// }

// TEST_CASE("Retrieve Statement Follows Call") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     CallStatement *callStatement = new CallStatement(1);
//     AssignStatement *assignStatement = new AssignStatement(2);
//     AssignStatement *assignStatement2 = new AssignStatement(3);
//     AssignStatement *assignStatement3 = new AssignStatement(4);
//     AssignStatement *assignStatement4 = new AssignStatement(5);
//     AssignStatement *assignStatement5 = new AssignStatement(6);
//     PrintStatement *printStatement = new PrintStatement(7);

//     std::shared_ptr<Relationship> relationship = std::make_shared<FollowsRelationship>(assignStatement, callStatement);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsRelationship>(assignStatement2, callStatement);
//     std::shared_ptr<Relationship> relationship3 = std::make_shared<FollowsRelationship>(assignStatement3, callStatement);
//     std::shared_ptr<Relationship> relationship4 = std::make_shared<FollowsRelationship>(assignStatement4, callStatement);
//     std::shared_ptr<Relationship> relationship5 = std::make_shared<FollowsRelationship>(assignStatement5, callStatement);
//     std::shared_ptr<Relationship> relationship6 = std::make_shared<FollowsRelationship>(printStatement, assignStatement5);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship3));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship4));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship5));
//     relationshipManager->storeRelationship(std::shared_ptr<Relationship> (relationship6));

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               CALL_STATEMENT);

//     REQUIRE(relationships->size() == 5);
//     for (Relationship *relationship : *relationships) {
//         bool isInRelationships = false;
//         for (Relationship *relationship2 : *relationships) {
//             if (PKBtestHelpers::relationshipEqualsRelationship(relationship, relationship2)) {
//                 isInRelationships = true;
//                 break;
//             }
//         }
//         REQUIRE(isInRelationships);
//     }

// //     delete callStatement;
//     delete assignStatement;
//     delete assignStatement2;
//     delete assignStatement3;
//     delete assignStatement4;
//     delete assignStatement5;
//     delete printStatement;
// }

// TEST_CASE("Duplicate Relationship has no effect") {
// auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);

//     std::shared_ptr<Relationship> relationship = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
//     std::shared_ptr<Relationship> relationship2 = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);

//     relationshipManager->storeRelationship(relationship))
//     relationshipManager->storeRelationship(relationship2);
//     REQUIRE(relationshipManager->getRelationshipsVectorByTypes(FOLLOWS_STAR, WHILE_STATEMENT, WHILE_STATEMENT)->size() == 1);
//     REQUIRE(relationshipManager->getRelationshipsVectorByTypes(FOLLOWS_STAR, WHILE_STATEMENT, WHILE_STATEMENT)->at(0) ==
//             relationship.get());

//     RelationshipLiteralHashkeyGenerator generator;

//     std::string stmtNumber1 = "1";
//     std::string stmtNumber2 = "2";

//     WhileStatement *whileStatementClone = new WhileStatement(1);
//     WhileStatement *whileStatement2Clone = new WhileStatement(2);
//     FollowsStarRelationship *followsStarRelationshipKey = new FollowsStarRelationship(whileStatementClone, whileStatement2Clone);

//     REQUIRE(relationshipManager->getRelationshipIfExist(followsStarRelationshipKey) == relationship.get());

// //     delete whileStatement;
//     delete whileStatement2;
//     delete whileStatementClone;
//     delete whileStatement2Clone;
//     delete followsStarRelationshipKey;
// }

TEST_CASE("Can retrieve relationship by literal values") {
    auto relationshipManagerOwner = std::make_shared<RelationshipManager>();
    RelationshipManager *relationshipManager = relationshipManagerOwner.get();

    auto whileStatementOwner = std::make_shared<WhileStatement>(1);
    auto whileStatementOwner2 = std::make_shared<WhileStatement>(2);
    auto assignStatementOwner = std::make_shared<AssignStatement>(3);
    auto assignStatementOwner2 = std::make_shared<AssignStatement>(4);
    auto callStatementOwner = std::make_shared<CallStatement>(5);
    auto readStatementOwner = std::make_shared<ReadStatement>(6);
    auto printStatementOwner = std::make_shared<PrintStatement>(7);
    auto ifStatementOwner = std::make_shared<IfStatement>(8);
    auto variableOwner = std::make_shared<Variable>(std::make_shared<std::string>("x"));
    auto variableOwner2 = std::make_shared<Variable>(std::make_shared<std::string>("y"));
    auto procedureOwner = std::make_shared<Procedure>(std::make_shared<std::string>("main"));

    WhileStatement *whileStatement = whileStatementOwner.get();
    WhileStatement *whileStatement2 = whileStatementOwner2.get();
    AssignStatement *assignStatement = assignStatementOwner.get();
    AssignStatement *assignStatement2 = assignStatementOwner2.get();
    CallStatement *callStatement = callStatementOwner.get();
    ReadStatement *readStatement = readStatementOwner.get();
    PrintStatement *printStatement = printStatementOwner.get();
    IfStatement *ifStatement = ifStatementOwner.get();
    Variable *variable = variableOwner.get();
    Variable *variable2 = variableOwner2.get();
    Procedure *procedure = procedureOwner.get();

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

    auto relationshipKey1 = RelationshipKey(&relationship->getRelationshipType(), &relationship->getLeftHandEntity()->getEntityKey(),
                                            &relationship->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship->equals(relationshipManager->getRelationship(relationshipKey1)));

    auto relationshipKey2 = RelationshipKey(&relationship2->getRelationshipType(), &relationship2->getLeftHandEntity()->getEntityKey(),
                                            &relationship2->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship2->equals(relationshipManager->getRelationship(relationshipKey2)));

    auto relationshipKey3 = RelationshipKey(&relationship3->getRelationshipType(), &relationship3->getLeftHandEntity()->getEntityKey(),
                                            &relationship3->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship3->equals(relationshipManager->getRelationship(relationshipKey3)));

    auto relationshipKey4 = RelationshipKey(&relationship4->getRelationshipType(), &relationship4->getLeftHandEntity()->getEntityKey(),
                                            &relationship4->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship4->equals(relationshipManager->getRelationship(relationshipKey4)));

    auto relationshipKey5 = RelationshipKey(&relationship5->getRelationshipType(), &relationship5->getLeftHandEntity()->getEntityKey(),
                                            &relationship5->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship5->equals(relationshipManager->getRelationship(relationshipKey5)));

    auto relationshipKey6 = RelationshipKey(&relationship6->getRelationshipType(), &relationship6->getLeftHandEntity()->getEntityKey(),
                                            &relationship6->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship6->equals(relationshipManager->getRelationship(relationshipKey6)));

    auto relationshipKey7 = RelationshipKey(&relationship7->getRelationshipType(), &relationship7->getLeftHandEntity()->getEntityKey(),
                                            &relationship7->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship7->equals(relationshipManager->getRelationship(relationshipKey7)));

    auto relationshipKey9 = RelationshipKey(&relationship9->getRelationshipType(), &relationship9->getLeftHandEntity()->getEntityKey(),
                                            &relationship9->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship9->equals(relationshipManager->getRelationship(relationshipKey9)));

    auto relationshipKey10 = RelationshipKey(&relationship10->getRelationshipType(), &relationship10->getLeftHandEntity()->getEntityKey(),
                                             &relationship10->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship10->equals(relationshipManager->getRelationship(relationshipKey10)));

    auto relationshipKey11 = RelationshipKey(&relationship11->getRelationshipType(), &relationship11->getLeftHandEntity()->getEntityKey(),
                                             &relationship11->getRightHandEntity()->getEntityKey());

    REQUIRE(relationship11->equals(relationshipManager->getRelationship(relationshipKey11)));
}
