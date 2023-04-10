// //
// // Created by Aiken Wong on 4/2/23.
// //

#include <catch2/catch_test_macros.hpp>
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

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST_CASE("RelationshipManager instantiates") {
  EntityManager entityManager;
  RelationshipManager relationshipManager = RelationshipManager(&entityManager);
}

TEST_CASE("RelationshipManager stores relationship") {
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();

  auto readStatement = std::make_unique<ReadStatement>(1);
  auto readStatement2 = std::make_unique<ReadStatement>(2);

  relationshipManager->storeRelationship(std::make_unique<FollowsRelationship>(readStatement.get(), readStatement2.get()));
}

TEST_CASE("RelationshipManager retrieves relationship") {
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();
  auto readStatement = std::make_unique<ReadStatement>(1);
  auto readStatement2 = std::make_unique<ReadStatement>(2);

  auto relationship = std::make_unique<FollowsRelationship>(readStatement.get(), readStatement2.get());
  auto *relationshipPtr = relationship.get();
  relationshipManager->storeRelationship(std::move(relationship));

  std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
      FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
      ReadStatement::getEntityTypeStatic());

  REQUIRE(relationships->size() == 1);
  REQUIRE(relationships->at(0)->equals(relationshipPtr));
}

TEST_CASE("RelationshipManager retrieves multitple relationships") {
  EntityManager entityManager;

  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();
  auto readStatement = std::make_unique<ReadStatement>(1);
  auto readStatement2 = std::make_unique<ReadStatement>(2);
  auto readStatement3 = std::make_unique<ReadStatement>(3);
  auto readStatement4 = std::make_unique<ReadStatement>(4);

  auto relationship = std::make_unique<FollowsRelationship>(readStatement.get(), readStatement2.get());
  auto *relationshipPtr = relationship.get();

  auto relationship2 = std::make_unique<FollowsRelationship>(readStatement3.get(), readStatement4.get());
  auto *relationship2Ptr = relationship2.get();
  relationshipManager->storeRelationship(std::move(relationship));
  relationshipManager->storeRelationship(std::move(relationship2));

  std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
      FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
      ReadStatement::getEntityTypeStatic());

  REQUIRE(relationships->size() == 2);
  REQUIRE(relationships->at(0)->equals(relationshipPtr));
  REQUIRE(relationships->at(1)->equals(relationship2Ptr));
}

TEST_CASE("RelationshipManager returns empty vector if no entries") {
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();

  std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
      FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
      ReadStatement::getEntityTypeStatic());

  REQUIRE(relationships->empty());
}

TEST_CASE("RelationshipManager returns empty vector if no entries of type") {
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();
  auto readStatement = std::make_unique<ReadStatement>(1);
  auto readStatement2 = std::make_unique<ReadStatement>(2);

  auto relationship = std::make_unique<FollowsRelationship>(readStatement.get(), readStatement2.get());

  relationshipManager->storeRelationship(std::move(relationship));

  std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
      FollowsStarRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
      ReadStatement::getEntityTypeStatic());

  REQUIRE(relationships->empty());
}

TEST_CASE("RelationshipManager can retrieve mulitple relationships of multiple types") {
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();
  auto readStatement = std::make_unique<ReadStatement>(1);
  auto readStatement2 = std::make_unique<ReadStatement>(2);
  auto readStatement3 = std::make_unique<ReadStatement>(3);
  auto readStatement4 = std::make_unique<ReadStatement>(4);
  auto assignStatement = std::make_unique<AssignStatement>(5);
  auto assignStatement2 = std::make_unique<AssignStatement>(6);

  auto aString = std::make_unique<std::string>("a");
  auto bString = std::make_unique<std::string>("b");
  auto variable = std::make_unique<Variable>(std::move(aString));
  auto variable2 = std::make_unique<Variable>(std::move(bString));

  auto relationship = std::make_unique<FollowsRelationship>(readStatement.get(), readStatement2.get());
  auto relationship2 = std::make_unique<FollowsRelationship>(readStatement3.get(), readStatement4.get());
  auto relationship3 = std::make_unique<ModifiesRelationship>(assignStatement.get(), variable.get());
  auto relationship4 = std::make_unique<ModifiesRelationship>(assignStatement2.get(), variable2.get());

  auto *relationshipPtr = relationship.get();
  auto *relationship2Ptr = relationship2.get();
  auto *relationship3Ptr = relationship3.get();
  auto *relationship4Ptr = relationship4.get();

  relationshipManager->storeRelationship(std::move(relationship));
  relationshipManager->storeRelationship(std::move(relationship2));
  relationshipManager->storeRelationship(std::move(relationship3));
  relationshipManager->storeRelationship(std::move(relationship4));

  std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(
      FollowsRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(),
      ReadStatement::getEntityTypeStatic());

  REQUIRE(relationships->size() == 2);
  REQUIRE(relationshipPtr->equals(relationships->at(0)));
  REQUIRE(relationship2Ptr->equals(relationships->at(1)));
  std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(
      ModifiesRelationship::getRelationshipTypeStatic(), AssignStatement::getEntityTypeStatic(),
      Variable::getEntityTypeStatic());

  REQUIRE(relationships2->size() == 2);
  REQUIRE(relationship3Ptr->equals(relationships2->at(0)));
  REQUIRE(relationship4Ptr->equals(relationships2->at(1)));
}

// TEST_CASE("Retrieve Statement follow Statement") {
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     ReadStatement *readStatement3 = new ReadStatement(3);
//     ReadStatement *readStatement4 = new ReadStatement(4);

//     PrintStatement *printStatement = new PrintStatement(3);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);
//     PrintStatement *printStatement4 = new PrintStatement(6);

//     std::shared_ptr<FollowsRelationship> relationship = std::make_unique<FollowsRelationship>(readStatement, readStatement2);
// autoonship> relationship2 = std::make_unique<FollowsRelationship>(readStatement3, printStatement);
// autoonship> relationship3 = std::make_unique<FollowsRelationship>(printStatement2, printStatement3);
// autoonship> relationship4 = std::make_unique<FollowsRelationship>(printStatement4, readStatement4);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     ReadStatement *readStatement3 = new ReadStatement(3);
//     ReadStatement *readStatement4 = new ReadStatement(4);

//     PrintStatement *printStatement = new PrintStatement(5);
//     PrintStatement *printStatement2 = new PrintStatement(6);
//     PrintStatement *printStatement3 = new PrintStatement(7);
//     PrintStatement *printStatement4 = new PrintStatement(8);

//     std::shared_ptr<FollowsRelationship> relationship = std::make_unique<FollowsRelationship>(readStatement, readStatement2);
// autoonship> relationship2 = std::make_unique<FollowsRelationship>(readStatement3, printStatement);
// autoonship> relationship3 = std::make_unique<FollowsRelationship>(printStatement2, printStatement3);
// autoonship> relationship4 = std::make_unique<FollowsRelationship>(printStatement4, readStatement4);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     IfStatement *ifStatement = new IfStatement(1);
//     WhileStatement *whileStatement = new WhileStatement(2);
//     WhileStatement *whileStatement2 = new WhileStatement(3);
//     ReadStatement *readStatement = new ReadStatement(4);

//     std::shared_ptr<FollowsRelationship> followsRelationship = std::make_unique<FollowsRelationship>(ifStatement, whileStatement);
//     std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_unique<FollowsRelationship>(whileStatement2, readStatement);

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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);
//     WhileStatement *whileStatement3 = new WhileStatement(3);
//     WhileStatement *whileStatement4 = new WhileStatement(4);
//     WhileStatement *whileStatement5 = new WhileStatement(5);
//     WhileStatement *whileStatement6 = new WhileStatement(6);

// autoonship> relationship = std::make_unique<ParentRelationship>(whileStatement, whileStatement2);
// autoonship> relationship2 = std::make_unique<ParentRelationship>(whileStatement3, whileStatement4);
// autoonship> relationship3 = std::make_unique<ParentRelationship>(whileStatement5, whileStatement6);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     CallStatement *callStatement = new CallStatement(1);
//     CallStatement *callStatement2 = new CallStatement(2);
//     CallStatement *callStatement3 = new CallStatement(3);
//     CallStatement *callStatement4 = new CallStatement(4);
//     CallStatement *callStatement5 = new CallStatement(5);
//     CallStatement *callStatement6 = new CallStatement(6);

// autoonship> relationship = std::make_unique<FollowsRelationship>(callStatement, callStatement2);
// autoonship> relationship2 = std::make_unique<FollowsRelationship>(callStatement3, callStatement4);
// autoonship> relationship3 = std::make_unique<FollowsRelationship>(callStatement5, callStatement6);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     PrintStatement *printStatement = new PrintStatement(3);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);

//     AssignStatement *assignStatement = new AssignStatement(6);
//     AssignStatement *assignStatement2 = new AssignStatement(7);
//     AssignStatement *assignStatement4 = new AssignStatement(9);

// autoonship> relationship = std::make_unique<ParentRelationship>(readStatement, assignStatement);
// autoonship> relationship2 = std::make_unique<ParentRelationship>(readStatement2, assignStatement2);
// autoonship> relationship3 = std::make_unique<ParentRelationship>(printStatement, printStatement2);
// autoonship> relationship4 = std::make_unique<ParentRelationship>(printStatement3, assignStatement4);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     ReadStatement *readStatement = new ReadStatement(1);
//     ReadStatement *readStatement2 = new ReadStatement(2);
//     PrintStatement *printStatement2 = new PrintStatement(4);
//     PrintStatement *printStatement3 = new PrintStatement(5);

//     AssignStatement *assignStatement = new AssignStatement(6);
//     AssignStatement *assignStatement2 = new AssignStatement(7);
//     AssignStatement *assignStatement3 = new AssignStatement(8);
//     AssignStatement *assignStatement4 = new AssignStatement(9);

// autoonship> relationship = std::make_unique<ParentRelationship>(readStatement, assignStatement);
// autoonship> relationship2 = std::make_unique<ParentRelationship>(readStatement2, assignStatement2);
// autoonship> relationship3 = std::make_unique<ParentRelationship>(assignStatement3, printStatement2);
// autoonship> relationship4 = std::make_unique<ParentRelationship>(printStatement3, assignStatement4);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();

//     std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsVectorByTypes(FOLLOWS, STATEMENT,
//                                                                                               STATEMENT);
//     REQUIRE(relationships->size() == 0);

// // }

// TEST_CASE("Retrieve While FollowsStar While") {
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);
//     WhileStatement *whileStatement3 = new WhileStatement(3);
//     WhileStatement *whileStatement4 = new WhileStatement(4);
//     WhileStatement *whileStatement5 = new WhileStatement(5);
//     WhileStatement *whileStatement6 = new WhileStatement(6);

// autoonship> relationship = std::make_unique<FollowsStarRelationship>(whileStatement, whileStatement2);
// autoonship> relationship2 = std::make_unique<FollowsStarRelationship>(whileStatement3, whileStatement4);
// autoonship> relationship3 = std::make_unique<FollowsStarRelationship>(whileStatement5, whileStatement6);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     PrintStatement *printStatement = new PrintStatement(2);
//     AssignStatement *assignStatement = new AssignStatement(3);
//     WhileStatement *whileStatement2 = new WhileStatement(4);
//     WhileStatement *whileStatement3 = new WhileStatement(5);
//     IfStatement *ifStatement = new IfStatement(6);
//     CallStatement *callStatement = new CallStatement(7);

// autoonship> relationship = std::make_unique<FollowsStarRelationship>(whileStatement, printStatement);
// autoonship> relationship2 = std::make_unique<FollowsStarRelationship>(whileStatement2, assignStatement);
// autoonship> relationship3 = std::make_unique<FollowsStarRelationship>(whileStatement3, whileStatement);
// autoonship> relationship4 = std::make_unique<FollowsStarRelationship>(whileStatement3, whileStatement2);
// autoonship> relationship5 = std::make_unique<FollowsStarRelationship>(whileStatement3, ifStatement);
// autoonship> relationship6 = std::make_unique<FollowsStarRelationship>(ifStatement, callStatement);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     IfStatement *ifStatement = new IfStatement(1);
//     CallStatement *callStatement = new CallStatement(2);
//     CallStatement *callStatement2 = new CallStatement(3);
//     CallStatement *callStatement3 = new CallStatement(4);
//     CallStatement *callStatement4 = new CallStatement(5);
//     CallStatement *callStatement5 = new CallStatement(6);
//     PrintStatement *printStatement = new PrintStatement(7);

// autoonship> relationship = std::make_unique<ParentStarRelationship>(ifStatement, callStatement);
// autoonship> relationship2 = std::make_unique<ParentStarRelationship>(ifStatement, callStatement2);
// autoonship> relationship3 = std::make_unique<ParentStarRelationship>(ifStatement, callStatement3);
// autoonship> relationship4 = std::make_unique<ParentStarRelationship>(ifStatement, callStatement4);
// autoonship> relationship5 = std::make_unique<ParentStarRelationship>(ifStatement, callStatement5);
// autoonship> relationship6 = std::make_unique<ParentStarRelationship>(ifStatement, printStatement);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     CallStatement *callStatement = new CallStatement(1);
//     AssignStatement *assignStatement = new AssignStatement(2);
//     AssignStatement *assignStatement2 = new AssignStatement(3);
//     AssignStatement *assignStatement3 = new AssignStatement(4);
//     AssignStatement *assignStatement4 = new AssignStatement(5);
//     AssignStatement *assignStatement5 = new AssignStatement(6);
//     PrintStatement *printStatement = new PrintStatement(7);

// autoonship> relationship = std::make_unique<FollowsRelationship>(assignStatement, callStatement);
// autoonship> relationship2 = std::make_unique<FollowsRelationship>(assignStatement2, callStatement);
// autoonship> relationship3 = std::make_unique<FollowsRelationship>(assignStatement3, callStatement);
// autoonship> relationship4 = std::make_unique<FollowsRelationship>(assignStatement4, callStatement);
// autoonship> relationship5 = std::make_unique<FollowsRelationship>(assignStatement5, callStatement);
// autoonship> relationship6 = std::make_unique<FollowsRelationship>(printStatement, assignStatement5);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
// EntityManager entityManager;
// auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
//     RelationshipManager *relationshipManagerelationshipManagerOwner.get();r();
//     WhileStatement *whileStatement = new WhileStatement(1);
//     WhileStatement *whileStatement2 = new WhileStatement(2);

// autoonship> relationship = std::make_unique<FollowsStarRelationship>(whileStatement, whileStatement2);
// autoonship> relationship2 = std::make_unique<FollowsStarRelationship>(whileStatement, whileStatement2);

//     relationshipManager->storeRelationship(std::move(relationship)))
//     relationshipManager->storeRelationship(std::move(relationship2));
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
  EntityManager entityManager;
  auto relationshipManagerOwner = std::make_unique<RelationshipManager>(&entityManager);
  RelationshipManager *relationshipManager = relationshipManagerOwner.get();

  auto whileStatementOwner = std::make_unique<WhileStatement>(1);
  auto whileStatementOwner2 = std::make_unique<WhileStatement>(2);
  auto assignStatementOwner = std::make_unique<AssignStatement>(3);
  auto assignStatementOwner2 = std::make_unique<AssignStatement>(4);
  auto callStatementOwner = std::make_unique<CallStatement>(5);
  auto readStatementOwner = std::make_unique<ReadStatement>(6);
  auto printStatementOwner = std::make_unique<PrintStatement>(7);
  auto ifStatementOwner = std::make_unique<IfStatement>(8);
  auto variableOwner = std::make_unique<Variable>(std::make_unique<std::string>("x"));
  auto variableOwner2 = std::make_unique<Variable>(std::make_unique<std::string>("y"));
  auto procedureOwner = std::make_unique<Procedure>(std::make_unique<std::string>("main"));

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

  auto relationship = std::make_unique<FollowsStarRelationship>(whileStatement, whileStatement2);
  auto relationship2 = std::make_unique<FollowsRelationship>(whileStatement, assignStatement);
  auto relationship3 = std::make_unique<ParentStarRelationship>(assignStatement, assignStatement2);
  auto relationship4 = std::make_unique<ParentRelationship>(assignStatement, callStatement);
  auto relationship5 = std::make_unique<UsesRelationship>(callStatement, variable);
  auto relationship6 = std::make_unique<ModifiesRelationship>(callStatement, variable2);
  auto relationship7 = std::make_unique<ModifiesRelationship>(readStatement, variable2);
  auto relationship9 = std::make_unique<UsesRelationship>(procedure, variable);
  auto relationship10 = std::make_unique<ModifiesRelationship>(ifStatement, variable2);
  auto relationship11 = std::make_unique<UsesRelationship>(printStatement, variable2);

  auto * relationshipPtr = relationship.get();
  auto * relationshipPtr2 = relationship2.get();
  auto * relationshipPtr3 = relationship3.get();
  auto * relationshipPtr4 = relationship4.get();
  auto * relationshipPtr5 = relationship5.get();
  auto * relationshipPtr6 = relationship6.get();
  auto * relationshipPtr7 = relationship7.get();
  auto * relationshipPtr9 = relationship9.get();
  auto * relationshipPtr10 = relationship10.get();
  auto * relationshipPtr11 = relationship11.get();

  relationshipManager->storeRelationship(std::move(relationship));
  relationshipManager->storeRelationship(std::move(relationship2));
  relationshipManager->storeRelationship(std::move(relationship3));
  relationshipManager->storeRelationship(std::move(relationship4));
  relationshipManager->storeRelationship(std::move(relationship5));
  relationshipManager->storeRelationship(std::move(relationship6));
  relationshipManager->storeRelationship(std::move(relationship7));
  relationshipManager->storeRelationship(std::move(relationship9));
  relationshipManager->storeRelationship(std::move(relationship10));
  relationshipManager->storeRelationship(std::move(relationship11));

  auto relationshipKey1 = RelationshipKey(&relationshipPtr->getRelationshipType(), &relationshipPtr->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr->equals(relationshipManager->getRelationship(relationshipKey1, nullptr, nullptr)));

  auto relationshipKey2 = RelationshipKey(&relationshipPtr2->getRelationshipType(), &relationshipPtr2->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr2->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr2->equals(relationshipManager->getRelationship(relationshipKey2, nullptr, nullptr)));

  auto relationshipKey3 = RelationshipKey(&relationshipPtr3->getRelationshipType(), &relationshipPtr3->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr3->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr3->equals(relationshipManager->getRelationship(relationshipKey3, nullptr, nullptr)));

  auto relationshipKey4 = RelationshipKey(&relationshipPtr4->getRelationshipType(), &relationshipPtr4->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr4->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr4->equals(relationshipManager->getRelationship(relationshipKey4, nullptr, nullptr)));

  auto relationshipKey5 = RelationshipKey(&relationshipPtr5->getRelationshipType(), &relationshipPtr5->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr5->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr5->equals(relationshipManager->getRelationship(relationshipKey5, nullptr, nullptr)));

  auto relationshipKey6 = RelationshipKey(&relationshipPtr6->getRelationshipType(), &relationshipPtr6->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr6->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr6->equals(relationshipManager->getRelationship(relationshipKey6, nullptr, nullptr)));

  auto relationshipKey7 = RelationshipKey(&relationshipPtr7->getRelationshipType(), &relationshipPtr7->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr7->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr7->equals(relationshipManager->getRelationship(relationshipKey7, nullptr, nullptr)));

  auto relationshipKey9 = RelationshipKey(&relationshipPtr9->getRelationshipType(), &relationshipPtr9->getLeftHandEntity()->getEntityKey(),
                                          &relationshipPtr9->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr9->equals(relationshipManager->getRelationship(relationshipKey9, nullptr, nullptr)));

  auto relationshipKey10 = RelationshipKey(&relationshipPtr10->getRelationshipType(), &relationshipPtr10->getLeftHandEntity()->getEntityKey(),
                                           &relationshipPtr10->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr10->equals(relationshipManager->getRelationship(relationshipKey10, nullptr, nullptr)));

  auto relationshipKey11 = RelationshipKey(&relationshipPtr11->getRelationshipType(), &relationshipPtr11->getLeftHandEntity()->getEntityKey(),
                                           &relationshipPtr11->getRightHandEntity()->getEntityKey());

  REQUIRE(relationshipPtr11->equals(relationshipManager->getRelationship(relationshipKey11, nullptr, nullptr)));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
