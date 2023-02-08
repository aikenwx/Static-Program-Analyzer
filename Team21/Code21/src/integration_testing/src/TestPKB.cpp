//
// Created by Aiken Wong on 5/2/23.
//

#include "catch.hpp"
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
    std::vector<ReadStatement *> *entities = queryFacade->getAllReadStatements();

    REQUIRE(entities->size() == 1);
    ReadStatement *expectedReadStatement = new ReadStatement(1);
    REQUIRE(entities->at(0)->equals(expectedReadStatement));

    delete pkb;
    delete expectedReadStatement;
    delete entities;
}

TEST_CASE("PKB can store multiple read statements and QueryFacade can retrieve read statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeReadStatement(1);
    populateFacade->storeReadStatement(2);
    std::vector<ReadStatement *> *entities = queryFacade->getAllReadStatements();

    REQUIRE(entities->size() == 2);
    ReadStatement *expectedReadStatement1 = new ReadStatement(1);
    ReadStatement *expectedReadStatement2 = new ReadStatement(2);
    REQUIRE(entities->at(0)->equals(expectedReadStatement1));
    REQUIRE(entities->at(1)->equals(expectedReadStatement2));

    delete pkb;
    delete expectedReadStatement1;
    delete expectedReadStatement2;
    delete entities;
}

TEST_CASE("PKB can store read relationship and QueryFacade can read retrieve relationship") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, READ_STATEMENT, 2, READ_STATEMENT);
    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            READ_STATEMENT, READ_STATEMENT);

    REQUIRE(relationships->size() == 1);

    FollowsRelationship *expectedFollowsRelationship = new FollowsRelationship(new ReadStatement(1),
                                                                               new ReadStatement(2));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedFollowsRelationship));

    delete pkb;
    delete expectedFollowsRelationship;
    delete relationships;
}

TEST_CASE("PKB can store multiple read follow read relationships and QueryFacade can retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, READ_STATEMENT, 2, READ_STATEMENT);
    populateFacade->storeFollowsRelationship(2, READ_STATEMENT, 3, READ_STATEMENT);
    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            READ_STATEMENT, READ_STATEMENT);

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedFollowsRelationship1 = new FollowsRelationship(new ReadStatement(1),
                                                                                new ReadStatement(2));
    FollowsRelationship *expectedFollowsRelationship2 = new FollowsRelationship(new ReadStatement(2),
                                                                                new ReadStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedFollowsRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedFollowsRelationship2));

    delete pkb;
    delete expectedFollowsRelationship1;
    delete expectedFollowsRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple print statements and retrieve mulitple print statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storePrintStatement(1);
    populateFacade->storePrintStatement(2);
    std::vector<PrintStatement *> *entities = queryFacade->getAllPrintStatements();

    REQUIRE(entities->size() == 2);
    PrintStatement *expectedPrintStatement1 = new PrintStatement(1);
    PrintStatement *expectedPrintStatement2 = new PrintStatement(2);
    REQUIRE(entities->at(0)->equals(expectedPrintStatement1));
    REQUIRE(entities->at(1)->equals(expectedPrintStatement2));

    delete pkb;
    delete expectedPrintStatement1;
    delete expectedPrintStatement2;
    delete entities;

}

TEST_CASE("PKB can store multiple print follow print relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, PRINT_STATEMENT, 2, PRINT_STATEMENT);
    populateFacade->storeFollowsRelationship(2, PRINT_STATEMENT, 3, PRINT_STATEMENT);
    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            PRINT_STATEMENT, PRINT_STATEMENT);

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedFollowsRelationship1 = new FollowsRelationship(new PrintStatement(1),
                                                                                new PrintStatement(2));
    FollowsRelationship *expectedFollowsRelationship2 = new FollowsRelationship(new PrintStatement(2),
                                                                                new PrintStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedFollowsRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedFollowsRelationship2));

    delete pkb;
    delete expectedFollowsRelationship1;
    delete expectedFollowsRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple call statements and retrieve multiple call statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeCallStatement(1);
    populateFacade->storeCallStatement(2);
    std::vector<CallStatement *> *entities = queryFacade->getAllCallStatements();

    REQUIRE(entities->size() == 2);
    CallStatement *expectedCallStatement1 = new CallStatement(1);
    CallStatement *expectedCallStatement2 = new CallStatement(2);
    REQUIRE(entities->at(0)->equals(expectedCallStatement1));
    REQUIRE(entities->at(1)->equals(expectedCallStatement2));

    delete pkb;
    delete expectedCallStatement1;
    delete expectedCallStatement2;
    delete entities;
}

TEST_CASE("PKB can store multiple assign statements and retrieve multiple assign statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignmentStatement(2);
    std::vector<AssignStatement *> *entities = queryFacade->getAllAssignStatements();

    REQUIRE(entities->size() == 2);
    AssignStatement *expectedAssignStatement1 = new AssignStatement(1);
    AssignStatement *expectedAssignStatement2 = new AssignStatement(2);
    REQUIRE(entities->at(0)->equals(expectedAssignStatement1));
    REQUIRE(entities->at(1)->equals(expectedAssignStatement2));

    delete pkb;
    delete expectedAssignStatement1;
    delete expectedAssignStatement2;
    delete entities;
}

TEST_CASE("PKB can store multiple if statements and retrieve multiple if statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeIfStatement(1);
    populateFacade->storeIfStatement(2);
    std::vector<IfStatement *> *entities = queryFacade->getAllIfStatements();

    REQUIRE(entities->size() == 2);
    IfStatement *expectedIfStatement1 = new IfStatement(1);
    IfStatement *expectedIfStatement2 = new IfStatement(2);
    REQUIRE(entities->at(0)->equals(expectedIfStatement1));
    REQUIRE(entities->at(1)->equals(expectedIfStatement2));

    delete pkb;
    delete expectedIfStatement1;
    delete expectedIfStatement2;
    delete entities;
}

TEST_CASE("PKB can store multiple while statements and retrieve multiple while statements") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeWhileStatement(1);
    populateFacade->storeWhileStatement(2);
    std::vector<WhileStatement *> *entities = queryFacade->getAllWhileStatements();

    REQUIRE(entities->size() == 2);
    WhileStatement *expectedWhileStatement1 = new WhileStatement(1);
    WhileStatement *expectedWhileStatement2 = new WhileStatement(2);
    REQUIRE(entities->at(0)->equals(expectedWhileStatement1));
    REQUIRE(entities->at(1)->equals(expectedWhileStatement2));

    delete pkb;
    delete expectedWhileStatement1;
    delete expectedWhileStatement2;
    delete entities;
}

TEST_CASE("PKB can store multiple while parent read relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 2, READ_STATEMENT);
    populateFacade->storeParentRelationship(2, WHILE_STATEMENT, 3, READ_STATEMENT);
    std::vector<ParentRelationship *> *relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
            WHILE_STATEMENT, READ_STATEMENT);

    REQUIRE(relationships->size() == 2);

    ParentRelationship *expectedParentRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                             new ReadStatement(2));
    ParentRelationship *expectedParentRelationship2 = new ParentRelationship(new WhileStatement(2),
                                                                             new ReadStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedParentRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedParentRelationship2));

    delete pkb;
    delete expectedParentRelationship1;
    delete expectedParentRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple while parent assign relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 2, ASSIGN_STATEMENT);
    populateFacade->storeParentRelationship(2, WHILE_STATEMENT, 3, ASSIGN_STATEMENT);
    std::vector<ParentRelationship *> *relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
            WHILE_STATEMENT, ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 2);

    ParentRelationship *expectedParentRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                             new AssignStatement(2));
    ParentRelationship *expectedParentRelationship2 = new ParentRelationship(new WhileStatement(2),
                                                                             new AssignStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedParentRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedParentRelationship2));

    delete pkb;
    delete expectedParentRelationship1;
    delete expectedParentRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store mulitple assignment follows read relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, ASSIGN_STATEMENT, 2, READ_STATEMENT);
    populateFacade->storeFollowsRelationship(2, ASSIGN_STATEMENT, 3, READ_STATEMENT);
    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            ASSIGN_STATEMENT, READ_STATEMENT);

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedFollowsRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                                new ReadStatement(2));
    FollowsRelationship *expectedFollowsRelationship2 = new FollowsRelationship(new AssignStatement(2),
                                                                                new ReadStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedFollowsRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedFollowsRelationship2));

    delete pkb;
    delete expectedFollowsRelationship1;
    delete expectedFollowsRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple assignment follows assignment relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, ASSIGN_STATEMENT, 2, ASSIGN_STATEMENT);
    populateFacade->storeFollowsRelationship(2, ASSIGN_STATEMENT, 3, ASSIGN_STATEMENT);
    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            ASSIGN_STATEMENT, ASSIGN_STATEMENT);

    REQUIRE(relationships->size() == 2);

    FollowsRelationship *expectedFollowsRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                                new AssignStatement(2));
    FollowsRelationship *expectedFollowsRelationship2 = new FollowsRelationship(new AssignStatement(2),
                                                                                new AssignStatement(3));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedFollowsRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedFollowsRelationship2));

    delete pkb;
    delete expectedFollowsRelationship1;
    delete expectedFollowsRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple assignment uses variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeStatementUsesVariableRelationship(1, ASSIGN_STATEMENT, "x");
    populateFacade->storeStatementUsesVariableRelationship(2, ASSIGN_STATEMENT, "y");
    std::vector<UsesRelationship *> *relationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(
            ASSIGN_STATEMENT, VARIABLE);

    REQUIRE(relationships->size() == 2);

    UsesRelationship *expectedUsesRelationship1 = new UsesRelationship(new AssignStatement(1), new Variable("x"));
    UsesRelationship *expectedUsesRelationship2 = new UsesRelationship(new AssignStatement(2), new Variable("y"));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedUsesRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedUsesRelationship2));

    delete pkb;
    delete expectedUsesRelationship1;
    delete expectedUsesRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple procedure uses variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeProcedureUsesVariableRelationship("Procedure1", "x");
    populateFacade->storeProcedureUsesVariableRelationship("Procedure2", "y");
    std::vector<UsesRelationship *> *relationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(
            PROCEDURE, VARIABLE);

    REQUIRE(relationships->size() == 2);

    UsesRelationship *expectedUsesRelationship1 = new UsesRelationship(new Procedure("Procedure1"), new Variable("x"));
    UsesRelationship *expectedUsesRelationship2 = new UsesRelationship(new Procedure("Procedure2"), new Variable("y"));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedUsesRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedUsesRelationship2));

    delete pkb;
    delete expectedUsesRelationship1;
    delete expectedUsesRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple call modifies variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeStatementModifiesVariableRelationship(1, CALL_STATEMENT, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, CALL_STATEMENT, "y");
    std::vector<ModifiesRelationship *> *relationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(
            CALL_STATEMENT, VARIABLE);

    REQUIRE(relationships->size() == 2);

    ModifiesRelationship *expectedModifiesRelationship1 = new ModifiesRelationship(new CallStatement(1),
                                                                                   new Variable("x"));
    ModifiesRelationship *expectedModifiesRelationship2 = new ModifiesRelationship(new CallStatement(2),
                                                                                   new Variable("y"));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedModifiesRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedModifiesRelationship2));

    delete pkb;
    delete expectedModifiesRelationship1;
    delete expectedModifiesRelationship2;
    delete relationships;
}

TEST_CASE("PKB can store multiple procedure modifies variable relationships and retrieve the relationships") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure2", "y");
    std::vector<ModifiesRelationship *> *relationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(
            PROCEDURE, VARIABLE);

    REQUIRE(relationships->size() == 2);

    ModifiesRelationship *expectedModifiesRelationship1 = new ModifiesRelationship(new Procedure("Procedure1"),
                                                                                   new Variable("x"));
    ModifiesRelationship *expectedModifiesRelationship2 = new ModifiesRelationship(new Procedure("Procedure2"),
                                                                                   new Variable("y"));

    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(0), expectedModifiesRelationship1));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(relationships->at(1), expectedModifiesRelationship2));

    delete pkb;
    delete expectedModifiesRelationship1;
    delete expectedModifiesRelationship2;
    delete relationships;
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

    std::vector<Statement *> *statements = queryFacade->getAllStatements();

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
    for (Statement *statement: *statements) {
        bool found = false;
        for (Statement *expectedStatement: expectedStatements) {
            found = expectedStatement->equals(statement);
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
    delete statements;
}

TEST_CASE("PKB can store mulitple types of statement follows statements relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeFollowsRelationship(1, ASSIGN_STATEMENT, 2, ASSIGN_STATEMENT);
    populateFacade->storeFollowsRelationship(2, ASSIGN_STATEMENT, 3, CALL_STATEMENT);
    populateFacade->storeFollowsRelationship(3, CALL_STATEMENT, 4, IF_STATEMENT);
    populateFacade->storeFollowsRelationship(4, IF_STATEMENT, 5, PRINT_STATEMENT);
    populateFacade->storeFollowsRelationship(5, PRINT_STATEMENT, 6, READ_STATEMENT);
    populateFacade->storeFollowsRelationship(6, READ_STATEMENT, 7, WHILE_STATEMENT);

    // should not be returned
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 2, ASSIGN_STATEMENT);
    populateFacade->storeStatementUsesVariableRelationship(1, ASSIGN_STATEMENT, "x");
    populateFacade->storeStatementModifiesVariableRelationship(1, ASSIGN_STATEMENT, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");

    std::vector<FollowsRelationship *> *relationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(
            STATEMENT, STATEMENT);

    REQUIRE(relationships->size() == 6);

    FollowsRelationship *expectedFollowsRelationship1 = new FollowsRelationship(new AssignStatement(1),
                                                                                new AssignStatement(2));
    FollowsRelationship *expectedFollowsRelationship2 = new FollowsRelationship(new AssignStatement(2),
                                                                                new CallStatement(3));
    FollowsRelationship *expectedFollowsRelationship3 = new FollowsRelationship(new CallStatement(3),
                                                                                new IfStatement(4));
    FollowsRelationship *expectedFollowsRelationship4 = new FollowsRelationship(new IfStatement(4),
                                                                                new PrintStatement(5));
    FollowsRelationship *expectedFollowsRelationship5 = new FollowsRelationship(new PrintStatement(5),
                                                                                new ReadStatement(6));
    FollowsRelationship *expectedFollowsRelationship6 = new FollowsRelationship(new ReadStatement(6),
                                                                                new WhileStatement(7));

    std::vector<FollowsRelationship *> expectedFollowsRelationships = {expectedFollowsRelationship1,
                                                                       expectedFollowsRelationship2,
                                                                       expectedFollowsRelationship3,
                                                                       expectedFollowsRelationship4,
                                                                       expectedFollowsRelationship5,
                                                                       expectedFollowsRelationship6};

    // for loop finds the relationship in the vector and compares it to the expected relationship
    for (FollowsRelationship *relationship: *relationships) {
        bool found = false;
        for (FollowsRelationship *expectedRelationship: expectedFollowsRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    delete expectedFollowsRelationship1;
    delete expectedFollowsRelationship2;
    delete expectedFollowsRelationship3;
    delete expectedFollowsRelationship4;
    delete expectedFollowsRelationship5;
    delete expectedFollowsRelationship6;
    delete relationships;
}

TEST_CASE("PKB can store multitple while parent statements relationships and retrieve them all") {
    PKB *pkb = new PKB();
    PopulateFacade *populateFacade = pkb->getPopulateFacade();
    QueryFacade *queryFacade = pkb->getQueryFacade();

    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 2, ASSIGN_STATEMENT);
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 3, CALL_STATEMENT);
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 4, PRINT_STATEMENT);
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 5, READ_STATEMENT);
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 6, IF_STATEMENT);
    populateFacade->storeParentRelationship(1, WHILE_STATEMENT, 7, WHILE_STATEMENT);

    // should not be returned
    populateFacade->storeParentRelationship(8, IF_STATEMENT, 9, ASSIGN_STATEMENT);
    populateFacade->storeFollowsRelationship(2, ASSIGN_STATEMENT, 3, CALL_STATEMENT);
    populateFacade->storeStatementUsesVariableRelationship(2, ASSIGN_STATEMENT, "x");
    populateFacade->storeStatementModifiesVariableRelationship(2, ASSIGN_STATEMENT, "y");
    populateFacade->storeProcedureModifiesVariableRelationship("Procedure1", "x");

    std::vector<ParentRelationship *> *relationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(
            WHILE_STATEMENT, STATEMENT);

    REQUIRE(relationships->size() == 6);

    ParentRelationship *expectedParentRelationship1 = new ParentRelationship(new WhileStatement(1),
                                                                             new AssignStatement(2));
    ParentRelationship *expectedParentRelationship2 = new ParentRelationship(new WhileStatement(1),
                                                                             new CallStatement(3));
    ParentRelationship *expectedParentRelationship3 = new ParentRelationship(new WhileStatement(1),
                                                                             new PrintStatement(4));
    ParentRelationship *expectedParentRelationship4 = new ParentRelationship(new WhileStatement(1),
                                                                             new ReadStatement(5));
    ParentRelationship *expectedParentRelationship5 = new ParentRelationship(new WhileStatement(1), new IfStatement(6));
    ParentRelationship *expectedParentRelationship6 = new ParentRelationship(new WhileStatement(1),
                                                                             new WhileStatement(7));

    std::vector<ParentRelationship *> expectedParentRelationships = {expectedParentRelationship1,
                                                                     expectedParentRelationship2,
                                                                     expectedParentRelationship3,
                                                                     expectedParentRelationship4,
                                                                     expectedParentRelationship5,
                                                                     expectedParentRelationship6};

    // for loop finds the relationship in the vector and compares it to the expected relationship

    for (ParentRelationship *relationship: *relationships) {
        bool found = false;
        for (ParentRelationship *expectedRelationship: expectedParentRelationships) {
            found = found || PKBtestHelpers::relationshipEqualsRelationship(relationship, expectedRelationship);
        }
        REQUIRE(found);
    }

    delete pkb;
    delete expectedParentRelationship1;
    delete expectedParentRelationship2;
    delete expectedParentRelationship3;
    delete expectedParentRelationship4;
    delete expectedParentRelationship5;
    delete expectedParentRelationship6;
    delete relationships;
}
