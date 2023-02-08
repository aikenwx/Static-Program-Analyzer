#include "catch.hpp"

#include "PKB/QueryFacade.h"
#include "PKBtestHelpers.h"


TEST_CASE("QueryFacade Instantiates") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);
    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve read statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    ReadStatement *readStatement = new ReadStatement(1);

    entityManager->storeEntity(readStatement);

    std::vector<ReadStatement *> *readStatements = queryFacade->getAllReadStatements();

    REQUIRE(readStatements->size() == 1);
    REQUIRE(readStatements->at(0)->equals(readStatement));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve print statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    PrintStatement *printStatement = new PrintStatement(1);

    entityManager->storeEntity(printStatement);

    std::vector<PrintStatement *> *printStatements = queryFacade->getAllPrintStatements();

    REQUIRE(printStatements->size() == 1);
    REQUIRE(printStatements->at(0)->equals(printStatement));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve assign statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    AssignStatement *assignStatement = new AssignStatement(1);

    entityManager->storeEntity(assignStatement);

    std::vector<AssignStatement *> *assignStatements = queryFacade->getAllAssignStatements();

    REQUIRE(assignStatements->size() == 1);
    REQUIRE(assignStatements->at(0)->equals(assignStatement));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve call statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    CallStatement *callStatement = new CallStatement(1);

    entityManager->storeEntity(callStatement);

    std::vector<CallStatement *> *callStatements = queryFacade->getAllCallStatements();

    REQUIRE(callStatements->size() == 1);
    REQUIRE(callStatements->at(0)->equals(callStatement));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve while statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    WhileStatement *whileStatement = new WhileStatement(1);

    entityManager->storeEntity(whileStatement);

    std::vector<WhileStatement *> *whileStatements = queryFacade->getAllWhileStatements();

    REQUIRE(whileStatements->size() == 1);
    REQUIRE(whileStatements->at(0)->equals(whileStatement));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve if statement") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);

    entityManager->storeEntity(ifStatement);

    std::vector<IfStatement *> *ifStatements = queryFacade->getAllIfStatements();

    REQUIRE(ifStatements->size() == 1);
    REQUIRE(ifStatements->at(0)->equals(ifStatement));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve procedure") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    Procedure *procedure = new Procedure("procedure");

    entityManager->storeEntity(procedure);

    std::vector<Procedure *> *procedures = queryFacade->getAllProcedures();

    REQUIRE(procedures->size() == 1);
    REQUIRE(procedures->at(0)->equals(procedure));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve variable") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    Variable *variable = new Variable("variable");

    entityManager->storeEntity(variable);

    std::vector<Variable *> *variables = queryFacade->getAllVariables();

    REQUIRE(variables->size() == 1);
    REQUIRE(variables->at(0)->equals(variable));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve constant") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    Constant *constant = new Constant(1);

    entityManager->storeEntity(constant);

    std::vector<Constant *> *constants = queryFacade->getAllConstants();

    REQUIRE(constants->size() == 1);
    REQUIRE(constants->at(0)->equals(constant));

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFace can retrieve all statements") {
    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(1);
    Variable *variable = new Variable("variable");

    entityManager->storeEntity(ifStatement);
    entityManager->storeEntity(whileStatement);
    entityManager->storeEntity(readStatement);
    entityManager->storeEntity(variable);

    std::vector<Statement *> *statements = queryFacade->getAllStatements();

    REQUIRE(statements->size() == 3);
    REQUIRE(Entity::isStatementType(statements->at(0)->getEntityType()));
    REQUIRE(Entity::isStatementType(statements->at(1)->getEntityType()));
    REQUIRE(Entity::isStatementType(statements->at(2)->getEntityType()));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}


TEST_CASE("QueryFacade can retrieve follow relationships") {

    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(1);

    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, whileStatement);
    FollowsRelationship *followsRelationship2 = new FollowsRelationship(whileStatement, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<FollowsRelationship *> *followsRelationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, WHILE_STATEMENT);
    REQUIRE(followsRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(0), followsRelationship));

    std::vector<FollowsRelationship *> *followsRelationships2 = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, READ_STATEMENT);
    REQUIRE(followsRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships2->at(0), followsRelationship2));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve parent relationships") {

    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(1);

    ParentRelationship *parentRelationship = new ParentRelationship(ifStatement, whileStatement);
    ParentRelationship *parentRelationship2 = new ParentRelationship(whileStatement, readStatement);

    relationshipManager->storeRelationship(parentRelationship);
    relationshipManager->storeRelationship(parentRelationship2);

    std::vector<ParentRelationship *> *parentRelationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, WHILE_STATEMENT);
    REQUIRE(parentRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(parentRelationships->at(0), parentRelationship));

    std::vector<ParentRelationship *> *parentRelationships2 = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, READ_STATEMENT);
    REQUIRE(parentRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(parentRelationships2->at(0), parentRelationship2));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve modifies relationships") {

    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(1);
    Variable *variable = new Variable("variable");
    Variable *variable2 = new Variable("variable2");

    ModifiesRelationship *modifiesRelationship = new ModifiesRelationship(ifStatement, variable);
    ModifiesRelationship *modifiesRelationship2 = new ModifiesRelationship(whileStatement, variable2);

    relationshipManager->storeRelationship(modifiesRelationship);
    relationshipManager->storeRelationship(modifiesRelationship2);

    std::vector<ModifiesRelationship *> *modifiesRelationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(modifiesRelationships->at(0), modifiesRelationship));

    std::vector<ModifiesRelationship *> *modifiesRelationships2 = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(modifiesRelationships2->at(0), modifiesRelationship2));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve uses relationships") {

    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    ReadStatement *readStatement = new ReadStatement(1);
    Variable *variable = new Variable("variable");
    Variable *variable2 = new Variable("variable2");

    UsesRelationship *usesRelationship = new UsesRelationship(ifStatement, variable);
    UsesRelationship *usesRelationship2 = new UsesRelationship(whileStatement, variable2);

    relationshipManager->storeRelationship(usesRelationship);
    relationshipManager->storeRelationship(usesRelationship2);

    std::vector<UsesRelationship *> *usesRelationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
    REQUIRE(usesRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(usesRelationships->at(0), usesRelationship));

    std::vector<UsesRelationship *> *usesRelationships2 = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
    REQUIRE(usesRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(usesRelationships2->at(0), usesRelationship2));


    delete entityManager;
    delete relationshipManager;
    delete queryFacade;
}

TEST_CASE("QueryFacade gives emtpy vector") {

        EntityManager* entityManager = new EntityManager();
        RelationshipManager* relationshipManager = new RelationshipManager();
        QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

        std::vector<ModifiesRelationship *> *modifiesRelationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
        REQUIRE(modifiesRelationships->size() == 0);

        std::vector<ModifiesRelationship *> *modifiesRelationships2 = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
        REQUIRE(modifiesRelationships2->size() == 0);


        delete entityManager;
        delete relationshipManager;
        delete queryFacade;
}

TEST_CASE("QueryFacade can retrieve Statement Follows Statement relationships") {

    EntityManager* entityManager = new EntityManager();
    RelationshipManager* relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);

    FollowsRelationship *followsRelationship = new FollowsRelationship(ifStatement, whileStatement);
    FollowsRelationship *followsRelationship2 = new FollowsRelationship(whileStatement, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);


    std::vector<FollowsRelationship *> *followsRelationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(STATEMENT, STATEMENT);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(0), followsRelationship));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(1), followsRelationship2));

    REQUIRE(followsRelationships->size() == 2);

    delete entityManager;
    delete relationshipManager;
    delete queryFacade;

}

