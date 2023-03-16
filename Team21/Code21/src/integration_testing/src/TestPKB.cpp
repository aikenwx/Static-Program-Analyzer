//
// Created by Aiken Wong on 5/2/23.
//
#include <catch2/catch_test_macros.hpp>

#include "PKB/PKB.h"
#include "PKBtestHelpers.h"

TEST_CASE("PKB instantiation") {
    PKB *pkb = new PKB();
    REQUIRE(pkb != nullptr);
    delete pkb;
}

TEST_CASE("Facades available") {
    PKB *pkb = new PKB();
    REQUIRE(pkb->getPopulateFacade() != nullptr);
    REQUIRE(pkb->getQueryFacade() != nullptr);
    delete pkb;
}

TEST_CASE("PKB can store read statement and QueryFacade can retrieve read statement") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeReadStatement(1);
    auto entities = queryFacade->getAllReadStatements();

    REQUIRE(entities->size() == 1);
    ReadStatement *expectedStatement = new ReadStatement(1);
    REQUIRE(entities->at(0)->equals(expectedStatement));

    delete pkb;
    delete expectedStatement;
}

TEST_CASE("PKB can store multiple read statements and QueryFacade can retrieve read statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeReadStatement(1);
    populateFacade->storeReadStatement(2);
    auto entities = queryFacade->getAllReadStatements();

    REQUIRE(entities->size() == 2);
    ReadStatement *expectedStatement1 = new ReadStatement(1);
    ReadStatement *expectedStatement2 = new ReadStatement(2);
    REQUIRE(entities->at(0)->equals(expectedStatement1));
    REQUIRE(entities->at(1)->equals(expectedStatement2));

    delete pkb;
    delete expectedStatement1;
    delete expectedStatement2;
}

TEST_CASE("PKB can store read relationship and QueryFacade can read retrieve relationship") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeReadStatement(1);
    populateFacade->storeReadStatement(2);
    populateFacade->storeFollowsRelationship(1, 2);
    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        ReadStatement::getEntityTypeStatic(), ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 1);

    FollowsRelationship *expectedRelationship = new FollowsRelationship(new ReadStatement(1),
                                                                        new ReadStatement(2));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship);
}

TEST_CASE("PKB can store multiple read follow read relationships and QueryFacade can retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeReadStatement(1);
    populateFacade->storeReadStatement(2);
    populateFacade->storeReadStatement(3);
    populateFacade->storeFollowsRelationship(1, 2);
    populateFacade->storeFollowsRelationship(2, 3);
    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        ReadStatement::getEntityTypeStatic(), ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedRelationship1 = new FollowsRelationship(new ReadStatement(1),
                                                                         new ReadStatement(2));
    FollowsRelationship *expectedRelationship2 = new FollowsRelationship(new ReadStatement(2),
                                                                         new ReadStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple print statements and retrieve mulitple print statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storePrintStatement(1);
    populateFacade->storePrintStatement(2);
    auto *entities = queryFacade->getAllPrintStatements();

    REQUIRE(entities->size() == 2);
    PrintStatement *expectedPrintStatement1 = new PrintStatement(1);
    PrintStatement *expectedPrintStatement2 = new PrintStatement(2);
    REQUIRE(entities->at(0)->equals(expectedPrintStatement1));
    REQUIRE(entities->at(1)->equals(expectedPrintStatement2));

    delete pkb;
    delete expectedPrintStatement1;
    delete expectedPrintStatement2;
}

TEST_CASE("PKB can store multiple print follow print relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storePrintStatement(1);
    populateFacade->storePrintStatement(2);
    populateFacade->storePrintStatement(3);
    populateFacade->storeFollowsRelationship(1, 2);
    populateFacade->storeFollowsRelationship(2, 3);
    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        PrintStatement::getEntityTypeStatic(), PrintStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedRelationship1 = new FollowsRelationship(new PrintStatement(1),
                                                                         new PrintStatement(2));
    FollowsRelationship *expectedRelationship2 = new FollowsRelationship(new PrintStatement(2),
                                                                         new PrintStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple call statements and retrieve multiple call statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeCallStatement(1);
    populateFacade->storeCallStatement(2);
    auto entities = queryFacade->getAllCallStatements();

    REQUIRE(entities->size() == 2);
    CallStatement *expectedCallStatement1 = new CallStatement(1);
    CallStatement *expectedCallStatement2 = new CallStatement(2);
    REQUIRE(entities->at(0)->equals(expectedCallStatement1));
    REQUIRE(entities->at(1)->equals(expectedCallStatement2));

    delete pkb;
    delete expectedCallStatement1;
    delete expectedCallStatement2;
}

TEST_CASE("PKB can store multiple assign statements and retrieve multiple assign statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignmentStatement(2);
    auto entities = queryFacade->getAllAssignStatements();

    REQUIRE(entities->size() == 2);
    AssignStatement *expectedAssignStatement1 = new AssignStatement(1);
    AssignStatement *expectedAssignStatement2 = new AssignStatement(2);
    REQUIRE(entities->at(0)->equals(expectedAssignStatement1));
    REQUIRE(entities->at(1)->equals(expectedAssignStatement2));

    delete pkb;
    delete expectedAssignStatement1;
    delete expectedAssignStatement2;
}

TEST_CASE("PKB can store multiple if statements and retrieve multiple if statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeIfStatement(1);
    populateFacade->storeIfStatement(2);
    auto entities = queryFacade->getAllIfStatements();

    REQUIRE(entities->size() == 2);
    IfStatement *expectedIfStatement1 = new IfStatement(1);
    IfStatement *expectedIfStatement2 = new IfStatement(2);
    REQUIRE(entities->at(0)->equals(expectedIfStatement1));
    REQUIRE(entities->at(1)->equals(expectedIfStatement2));

    delete pkb;
    delete expectedIfStatement1;
    delete expectedIfStatement2;
}

TEST_CASE("PKB can store multiple while statements and retrieve multiple while statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeWhileStatement(2);
    auto entities = queryFacade->getAllWhileStatements();

    REQUIRE(entities->size() == 2);
    WhileStatement *expectedWhileStatement1 = new WhileStatement(1);
    WhileStatement *expectedWhileStatement2 = new WhileStatement(2);
    REQUIRE(entities->at(0)->equals(expectedWhileStatement1));
    REQUIRE(entities->at(1)->equals(expectedWhileStatement2));

    delete pkb;
    delete expectedWhileStatement1;
    delete expectedWhileStatement2;
}

TEST_CASE("PKB can store multiple while parent read relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeReadStatement(2);
    populateFacade->storeReadStatement(3);
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeParentRelationship(1, 3);
    auto relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
        WhileStatement::getEntityTypeStatic(), ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    ParentRelationship *expectedRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                       new ReadStatement(2));
    ParentRelationship *expectedRelationship2 = new ParentRelationship(new WhileStatement(1),
                                                                       new ReadStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple while parent assign relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeWhileStatement(3);
    populateFacade->storeAssignmentStatement(4);
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeParentRelationship(3, 4);
    auto relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
        WhileStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    ParentRelationship *expectedRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                       new AssignStatement(2));
    ParentRelationship *expectedRelationship2 = new ParentRelationship(new WhileStatement(3),
                                                                       new AssignStatement(4));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store mulitple assignment follows read relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeReadStatement(2);
    populateFacade->storeAssignmentStatement(3);
    populateFacade->storeReadStatement(4);
    populateFacade->storeFollowsRelationship(1, 2);
    populateFacade->storeFollowsRelationship(3, 4);
    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        AssignStatement::getEntityTypeStatic(), ReadStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                         new ReadStatement(2));
    FollowsRelationship *expectedRelationship2 = new FollowsRelationship(new AssignStatement(3),
                                                                         new ReadStatement(4));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple assignment follows assignment relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeAssignmentStatement(3);
    populateFacade->storeFollowsRelationship(1, 2);
    populateFacade->storeFollowsRelationship(2, 3);
    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        AssignStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                         new AssignStatement(2));
    FollowsRelationship *expectedRelationship2 = new FollowsRelationship(new AssignStatement(2),
                                                                         new AssignStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple assignment uses variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeStatementUsesVariableRelationship(1, "x");
    populateFacade->storeStatementUsesVariableRelationship(2, "y");
    auto relationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(
        AssignStatement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    UsesRelationship *expectedRelationship1 = new UsesRelationship(new AssignStatement(1), new Variable(new std::string("x")));
    UsesRelationship *expectedRelationship2 = new UsesRelationship(new AssignStatement(2), new Variable(new std::string("y")));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple procedure uses variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeProcedureUsesVariableRelationship("Procedure1", "x");
    populateFacade->storeProcedureUsesVariableRelationship("Procedure2", "y");
    auto relationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(
        Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    UsesRelationship *expectedRelationship1 = new UsesRelationship(new Procedure(new std::string("Procedure1")), new Variable(new std::string("x")));
    UsesRelationship *expectedRelationship2 = new UsesRelationship(new Procedure(new std::string("Procedure2")), new Variable(new std::string("y")));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple call modifies variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeCallStatement(1);
    populateFacade->storeCallStatement(2);
    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeStatementModifiesVariableRelationship(1, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    auto relationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(
        CallStatement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    ModifiesRelationship *expectedRelationship1 = new ModifiesRelationship(new CallStatement(1),
                                                                           new Variable(new std::string("x")));
    ModifiesRelationship *expectedRelationship2 = new ModifiesRelationship(new CallStatement(2),
                                                                           new Variable(new std::string("y")));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store multiple procedure modifies variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure2", "y");
    auto relationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(
        Procedure::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    ModifiesRelationship *expectedRelationship1 = new ModifiesRelationship(new Procedure(new std::string("Procedure1")),
                                                                           new Variable(new std::string("x")));
    ModifiesRelationship *expectedRelationship2 = new ModifiesRelationship(new Procedure(new std::string("Procedure2")),
                                                                           new Variable(new std::string("y")));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedRelationship2));

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
}

TEST_CASE("PKB can store mulitple types of statements and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeCallStatement(2);
    populateFacade->storeIfStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeReadStatement(5);
    populateFacade->storeWhileStatement(6);
    populateFacade->storeAssignmentStatement(7);

    // should not be returned
    populateFacade->storeConstant(1);
    populateFacade->storeVariable("x");
    populateFacade->storeProcedure("Procedure1");

    auto statements = queryFacade->getAllStatements();

    REQUIRE(statements->size() == 7);

    Statement *expectedStatement1 = new AssignStatement(1);
    Statement *expectedStatement2 = new CallStatement(2);
    Statement *expectedStatement3 = new IfStatement(3);
    Statement *expectedStatement4 = new PrintStatement(4);
    Statement *expectedStatement5 = new ReadStatement(5);
    Statement *expectedStatement6 = new WhileStatement(6);
    Statement *expectedStatement7 = new AssignStatement(7);

    std::vector<Statement *> expectedStatements = {expectedStatement1, expectedStatement2, expectedStatement3,
                                                   expectedStatement4, expectedStatement5, expectedStatement6,
                                                   expectedStatement7};

    // for loop finds the statement in the vector and compares it to the expected statement
    for (auto statement : *statements) {
        bool found = false;
        for (Statement *expectedStatement : expectedStatements) {
            found = statement->equals(expectedStatement);
        }
    }

    delete pkb;
    delete expectedStatement1;
    delete expectedStatement2;
    delete expectedStatement3;
    delete expectedStatement4;
    delete expectedStatement5;
    delete expectedStatement6;
    delete expectedStatement7;
}

TEST_CASE("PKB can store mulitple types of statement follows statements relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storeIfStatement(4);
    populateFacade->storePrintStatement(5);
    populateFacade->storeReadStatement(6);
    populateFacade->storeWhileStatement(7);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeProcedure("Procedure1");

    populateFacade->storeFollowsRelationship(1, 2);
    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeFollowsRelationship(3, 4);
    populateFacade->storeFollowsRelationship(4, 5);
    populateFacade->storeFollowsRelationship(5, 6);
    populateFacade->storeFollowsRelationship(6, 7);

    // should not be returned
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeStatementUsesVariableRelationship(1, "x");
    populateFacade->storeStatementModifiesVariableRelationship(1, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");

    auto relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
        Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 6);

    FollowsRelationship *expectedRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                         new AssignStatement(2));
    FollowsRelationship *expectedRelationship2 = new FollowsRelationship(new AssignStatement(2),
                                                                         new CallStatement(3));
    FollowsRelationship *expectedRelationship3 = new FollowsRelationship(new CallStatement(3),
                                                                         new IfStatement(4));
    FollowsRelationship *expectedRelationship4 = new FollowsRelationship(new IfStatement(4),
                                                                         new PrintStatement(5));
    FollowsRelationship *expectedRelationship5 = new FollowsRelationship(new PrintStatement(5),
                                                                         new ReadStatement(6));
    FollowsRelationship *expectedRelationship6 = new FollowsRelationship(new ReadStatement(6),
                                                                         new WhileStatement(7));

    std::vector<FollowsRelationship *> expectedRelationships = {expectedRelationship1,
                                                                expectedRelationship2,
                                                                expectedRelationship3,
                                                                expectedRelationship4,
                                                                expectedRelationship5,
                                                                expectedRelationship6};

    // for loop finds the relationship in the vector and compares it to the expected relationship
    for (auto relationship : *relationships) {
        bool found = false;
        for (FollowsRelationship *expectedRelationship : expectedRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
    PKBtestHelpers::deleteRelationship(expectedRelationship3);
    PKBtestHelpers::deleteRelationship(expectedRelationship4);
    PKBtestHelpers::deleteRelationship(expectedRelationship5);
    PKBtestHelpers::deleteRelationship(expectedRelationship6);
}

TEST_CASE("PKB can store multitple while parent statements relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeReadStatement(5);
    populateFacade->storeIfStatement(6);
    populateFacade->storeWhileStatement(7);

    populateFacade->storeIfStatement(8);
    populateFacade->storeAssignmentStatement(9);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeProcedure("Procedure1");

    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeParentRelationship(1, 3);
    populateFacade->storeParentRelationship(1, 4);
    populateFacade->storeParentRelationship(1, 5);
    populateFacade->storeParentRelationship(1, 6);
    populateFacade->storeParentRelationship(1, 7);

    // should not be returned
    populateFacade->storeParentRelationship(8, 9);
    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeStatementUsesVariableRelationship(2, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeParentStarRelationship(1, 2);
    populateFacade->storeFollowsStarRelationship(2, 3);

    auto relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
        WhileStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 6);

    ParentRelationship *expectedRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                       new AssignStatement(2));
    ParentRelationship *expectedRelationship2 = new ParentRelationship(new WhileStatement(1),
                                                                       new CallStatement(3));
    ParentRelationship *expectedRelationship3 = new ParentRelationship(new WhileStatement(1),
                                                                       new PrintStatement(4));
    ParentRelationship *expectedRelationship4 = new ParentRelationship(new WhileStatement(1),
                                                                       new ReadStatement(5));
    ParentRelationship *expectedRelationship5 = new ParentRelationship(new WhileStatement(1), new IfStatement(6));
    ParentRelationship *expectedRelationship6 = new ParentRelationship(new WhileStatement(1),
                                                                       new WhileStatement(7));

    std::vector<ParentRelationship *> expectedRelationships = {expectedRelationship1,
                                                               expectedRelationship2,
                                                               expectedRelationship3,
                                                               expectedRelationship4,
                                                               expectedRelationship5,
                                                               expectedRelationship6};

    // for loop finds the relationship in the vector and compares it to the expected relationship

    for (auto relationship : *relationships) {
        bool found = false;
        for (ParentRelationship *expectedRelationship : expectedRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
    PKBtestHelpers::deleteRelationship(expectedRelationship3);
    PKBtestHelpers::deleteRelationship(expectedRelationship4);
    PKBtestHelpers::deleteRelationship(expectedRelationship5);
    PKBtestHelpers::deleteRelationship(expectedRelationship6);
}

TEST_CASE("PKB can store multitple while parentStar statements relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeReadStatement(5);
    populateFacade->storeIfStatement(6);
    populateFacade->storeWhileStatement(7);

    populateFacade->storeIfStatement(8);
    populateFacade->storeAssignmentStatement(9);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeProcedure("Procedure1");

    populateFacade->storeParentStarRelationship(1, 2);
    populateFacade->storeParentStarRelationship(1, 3);
    populateFacade->storeParentStarRelationship(1, 4);
    populateFacade->storeParentStarRelationship(1, 5);
    populateFacade->storeParentStarRelationship(1, 6);
    populateFacade->storeParentStarRelationship(1, 7);

    // should not be returned
    populateFacade->storeParentRelationship(8, 9);
    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeStatementUsesVariableRelationship(2, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeFollowsStarRelationship(2, 3);

    auto relationships = queryFacade->getParentStarRelationshipsByLeftAndRightEntityTypes(
        WhileStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 6);

    ParentStarRelationship *expectedRelationship1 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new AssignStatement(2));
    ParentStarRelationship *expectedRelationship2 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new CallStatement(3));
    ParentStarRelationship *expectedRelationship3 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new PrintStatement(4));
    ParentStarRelationship *expectedRelationship4 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new ReadStatement(5));
    ParentStarRelationship *expectedRelationship5 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new IfStatement(6));
    ParentStarRelationship *expectedRelationship6 = new ParentStarRelationship(new WhileStatement(1),
                                                                               new WhileStatement(7));

    std::vector<ParentStarRelationship *> expectedRelationships = {expectedRelationship1,
                                                                   expectedRelationship2,
                                                                   expectedRelationship3,
                                                                   expectedRelationship4,
                                                                   expectedRelationship5,
                                                                   expectedRelationship6};

    // for loop finds the relationship in the vector and compares it to the expected relationship

    for (auto relationship : *relationships) {
        bool found = false;
        for (ParentStarRelationship *expectedRelationship : expectedRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
    PKBtestHelpers::deleteRelationship(expectedRelationship3);
    PKBtestHelpers::deleteRelationship(expectedRelationship4);
    PKBtestHelpers::deleteRelationship(expectedRelationship5);
    PKBtestHelpers::deleteRelationship(expectedRelationship6);
}

TEST_CASE("PKB can store multiple Calls relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeReadStatement(5);
    populateFacade->storeIfStatement(6);
    populateFacade->storeWhileStatement(7);

    populateFacade->storeIfStatement(8);
    populateFacade->storeAssignmentStatement(9);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");

    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeProcedure("Procedure3");
    populateFacade->storeProcedure("Procedure4");
    populateFacade->storeProcedure("Procedure5");

    populateFacade->storeCallsRelationship("Procedure1", "Procedure2");
    populateFacade->storeCallsRelationship("Procedure1", "Procedure3");
    populateFacade->storeCallsRelationship("Procedure1", "Procedure4");
    populateFacade->storeCallsRelationship("Procedure1", "Procedure5");

    // should not be returned
    populateFacade->storeParentRelationship(8, 9);
    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeStatementUsesVariableRelationship(2, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeFollowsStarRelationship(2, 3);
    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure2");

    auto relationships = queryFacade->getAllCallsRelationships();

    REQUIRE(relationships->size() == 4);

    CallsRelationship *expectedRelationship1 = new CallsRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure2")));
    CallsRelationship *expectedRelationship2 = new CallsRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure3")));
    CallsRelationship *expectedRelationship3 = new CallsRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure4")));
    CallsRelationship *expectedRelationship4 = new CallsRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure5")));

    std::vector<CallsRelationship *> expectedRelationships = {expectedRelationship1,
                                                              expectedRelationship2,
                                                              expectedRelationship3,
                                                              expectedRelationship4};

    // for loop finds the relationship in the vector and compares it to the expected relationship

    for (auto relationship : *relationships) {
        bool found = false;
        for (CallsRelationship *expectedRelationship : expectedRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
    PKBtestHelpers::deleteRelationship(expectedRelationship3);
    PKBtestHelpers::deleteRelationship(expectedRelationship4);
}

TEST_CASE("PKB can store multiple CallsStar relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeReadStatement(5);
    populateFacade->storeIfStatement(6);
    populateFacade->storeWhileStatement(7);

    populateFacade->storeIfStatement(8);
    populateFacade->storeAssignmentStatement(9);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");

    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeProcedure("Procedure3");
    populateFacade->storeProcedure("Procedure4");
    populateFacade->storeProcedure("Procedure5");

    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure2");
    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure3");
    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure4");
    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure5");

    // should not be returned
    populateFacade->storeParentRelationship(8, 9);
    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeStatementUsesVariableRelationship(2, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeFollowsStarRelationship(2, 3);
    populateFacade->storeCallsRelationship("Procedure1", "Procedure2");

    auto relationships = queryFacade->getAllCallsStarRelationships();

    REQUIRE(relationships->size() == 4);

    CallsStarRelationship *expectedRelationship1 = new CallsStarRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure2")));
    CallsStarRelationship *expectedRelationship2 = new CallsStarRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure3")));
    CallsStarRelationship *expectedRelationship3 = new CallsStarRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure4")));
    CallsStarRelationship *expectedRelationship4 = new CallsStarRelationship(new Procedure(new std::string("Procedure1")), new Procedure(new std::string("Procedure5")));

    std::vector<CallsStarRelationship *> expectedRelationships = {expectedRelationship1,
                                                                  expectedRelationship2,
                                                                  expectedRelationship3,
                                                                  expectedRelationship4};

    // for loop finds the relationship in the vector and compares it to the expected relationship

    for (auto relationship : *relationships) {
        bool found = false;
        for (CallsStarRelationship *expectedRelationship : expectedRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    PKBtestHelpers::deleteRelationship(expectedRelationship1);
    PKBtestHelpers::deleteRelationship(expectedRelationship2);
    PKBtestHelpers::deleteRelationship(expectedRelationship3);
    PKBtestHelpers::deleteRelationship(expectedRelationship4);
}

TEST_CASE("PKB can store while and if statememt pattern information") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeIfStatement(2);
    populateFacade->storeWhileStatement(3);
    populateFacade->storeIfStatement(4);

    populateFacade->storeWhileStatementConditionVariable(1, "x");
    populateFacade->storeWhileStatementConditionVariable(1, "x");

    populateFacade->storeWhileStatementConditionVariable(3, "x");

    populateFacade->storeIfStatementConditionVariable(2, "y");
    populateFacade->storeIfStatementConditionVariable(2, "y");

    populateFacade->storeIfStatementConditionVariable(4, "y");

    std::unordered_set<WhileStatement *> *whileStatements = queryFacade->getWhileStatementsUsingVariableInCondition("x");

    REQUIRE(whileStatements->size() == 2);

    // convert to array
    std::vector<WhileStatement *> whileStatementsVector(whileStatements->begin(), whileStatements->end());
    whileStatementsVector[0]->equals(std::make_shared<WhileStatement>(1).get());
    whileStatementsVector[1]->equals(std::make_shared<WhileStatement>(3).get());

    whileStatements = queryFacade->getWhileStatementsUsingVariableInCondition("y");

    REQUIRE(whileStatements->size() == 0);

    std::unordered_set<IfStatement *> *ifStatements = queryFacade->getIfStatementsUsingVariableInCondition("y");

    REQUIRE(ifStatements->size() == 2);
    // convert to array

    std::vector<IfStatement *> ifStatementsVector(ifStatements->begin(), ifStatements->end());
    ifStatementsVector[0]->equals(std::make_shared<IfStatement>(2).get());
    ifStatementsVector[1]->equals(std::make_shared<IfStatement>(4).get());

    ifStatements = queryFacade->getIfStatementsUsingVariableInCondition("x");

    REQUIRE(ifStatements->size() == 0);

    delete pkb;
}

TEST_CASE("Can add procedure name to calls") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    // Procedure1 calls Procedure2
    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeCallStatement(1);
    populateFacade->storeAssignmentStatement(2);

    populateFacade->storeAssignStatementPostfixExpression(2, "ab-");

    populateFacade->storeCallStatementProcedureName(1, "Procedure2");

    populateFacade->storeAssignStatementPostfixExpression(2, "ab-");

    populateFacade->storeCallStatementProcedureName(1, "Procedure2");

    populateFacade->storeCallsRelationship("Procedure1", "Procedure2");

    CallStatement *callStatement = (CallStatement *)queryFacade->getEntity(CallStatement::getEntityTypeStatic(), 1);
    REQUIRE(*callStatement->getProcedureName() == "Procedure2");

    AssignStatement *assignStatement = (AssignStatement *)queryFacade->getEntity(AssignStatement::getEntityTypeStatic(), 2);

    REQUIRE(*assignStatement->getPostFixExpression() == "ab-");
    delete pkb;
}

TEST_CASE("PKB general query API works") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);
    populateFacade->storeWhileStatement(1);
    populateFacade->storeAssignmentStatement(2);
    populateFacade->storeCallStatement(3);
    populateFacade->storePrintStatement(4);

    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");
    populateFacade->storeVariable("x");
    populateFacade->storeVariable("y");

    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");
    populateFacade->storeProcedure("Procedure1");
    populateFacade->storeProcedure("Procedure2");

    populateFacade->storeParentRelationship(1, 2);
    populateFacade->storeParentRelationship(1, 3);
    populateFacade->storeParentRelationship(1, 4);

    populateFacade->storeFollowsRelationship(2, 3);
    populateFacade->storeFollowsRelationship(3, 4);

    populateFacade->storeFollowsStarRelationship(2, 3);

    populateFacade->storeStatementUsesVariableRelationship(2, "x");
    populateFacade->storeStatementUsesVariableRelationship(3, "y");
    populateFacade->storeStatementUsesVariableRelationship(3, "y");

    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeStatementModifiesVariableRelationship(2, "y");
    populateFacade->storeStatementModifiesVariableRelationship(3, "x");
    populateFacade->storeStatementModifiesVariableRelationship(3, "y");

    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");

    populateFacade->storeCallsRelationship("Procedure1", "Procedure2");

    populateFacade->storeCallsStarRelationship("Procedure1", "Procedure2");

    std::vector<Entity *> *entities = queryFacade->getEntitiesByType(Statement::getEntityTypeStatic());

    REQUIRE(entities->size() == 4);

    auto statement1 = queryFacade->getEntity(Statement::getEntityTypeStatic(), 1);
    auto statement3 = queryFacade->getEntity(CallStatement::getEntityTypeStatic(), 3);
    REQUIRE(entities->at(0)->equals(statement1));
    REQUIRE(entities->at(1)->equals(std::make_shared<AssignStatement>(2).get()));
    REQUIRE(entities->at(2)->equals(statement3));
    REQUIRE(entities->at(3)->equals(std::make_shared<PrintStatement>(4).get()));

    entities = queryFacade->getEntitiesByType(Variable::getEntityTypeStatic());

    REQUIRE(entities->size() == 2);

    auto variable1 = queryFacade->getEntity(Variable::getEntityTypeStatic(), "x");
    auto variable2 = queryFacade->getEntity(Variable::getEntityTypeStatic(), "y");

    REQUIRE(entities->at(0)->equals(variable1));
    REQUIRE(entities->at(1)->equals(variable2));

    std::vector<Relationship *> *relationships = queryFacade->getRelationshipsByTypes(FollowsRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

    REQUIRE(relationships->size() == 2);

    REQUIRE(relationships->at(0)->equals(std::make_shared<FollowsRelationship>(std::make_shared<AssignStatement>(2).get(), std::make_shared<CallStatement>(3).get()).get()));
    REQUIRE(relationships->at(1)->equals(std::make_shared<FollowsRelationship>(std::make_shared<CallStatement>(3).get(), std::make_shared<PrintStatement>(4).get()).get()));

    std::vector<Relationship *> *relationships2 = queryFacade->getRelationshipsByTypes(UsesRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships2->size() == 2);

    REQUIRE(relationships2->at(0)->equals(std::make_shared<UsesRelationship>(std::make_shared<AssignStatement>(2).get(), std::make_shared<Variable>(new std::string("x")).get()).get()));

    REQUIRE(relationships2->at(1)->equals(std::make_shared<UsesRelationship>(std::make_shared<CallStatement>(3).get(), std::make_shared<Variable>(new std::string("y")).get()).get()));

    std::vector<Relationship *> *relationships3 = queryFacade->getRelationshipsByTypes(ModifiesRelationship::getRelationshipTypeStatic(), CallStatement::getEntityTypeStatic(), Variable::getEntityTypeStatic());

    REQUIRE(relationships3->size() == 2);

    REQUIRE(relationships3->at(0)->equals(std::make_shared<ModifiesRelationship>(std::make_shared<CallStatement>(3).get(), std::make_shared<Variable>(new std::string("x")).get()).get()));

    std::vector<Entity *> *entities2 = queryFacade->getRelationshipsByLeftEntityLiteralAndRightEntityType(ModifiesRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), 3, Variable::getEntityTypeStatic());

    REQUIRE(entities2->size() == 2);

    auto result1 = std::make_shared<Variable>(new std::string("x"));
    REQUIRE(entities2->at(0)->equals(result1.get()));
    REQUIRE(entities2->at(1)->equals(std::make_shared<Variable>(new std::string("y")).get()));

    std::vector<Entity *> *entities3 = queryFacade->getRelationshipsByLeftEntityTypeAndRightEntityLiteral(UsesRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Variable::getEntityTypeStatic(), "x");

    REQUIRE(entities3->size() == 1);
    REQUIRE(entities3->at(0)->equals(std::make_shared<AssignStatement>(2).get()));

    delete pkb;
}
