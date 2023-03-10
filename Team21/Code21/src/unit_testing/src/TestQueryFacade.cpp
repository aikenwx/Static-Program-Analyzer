#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKB/QueryFacade.h"
#include "PKBtestHelpers.h"

TEST_CASE("QueryFacade Instantiates") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);
    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve read statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);

    entityManager->storeStatement(readStatement);

    std::vector<ReadStatement *> *readStatements = queryFacade->getAllReadStatements();

    REQUIRE(readStatements->size() == 1);
    REQUIRE(readStatements->at(0)->equals(readStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve print statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(1);
    entityManager->storeStatement(printStatement);

    std::vector<PrintStatement *> *printStatements = queryFacade->getAllPrintStatements();

    REQUIRE(printStatements->size() == 1);
    REQUIRE(printStatements->at(0)->equals(printStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve assign statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(1);
    entityManager->storeStatement(assignStatement);

    std::vector<AssignStatement *> *assignStatements = queryFacade->getAllAssignStatements();

    REQUIRE(assignStatements->size() == 1);
    REQUIRE(assignStatements->at(0)->equals(assignStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve call statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<CallStatement> callStatement = std::make_shared<CallStatement>(1);
    entityManager->storeStatement(callStatement);

    std::vector<CallStatement *> *callStatements = queryFacade->getAllCallStatements();

    REQUIRE(callStatements->size() == 1);
    REQUIRE(callStatements->at(0)->equals(callStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve while statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(1);

    entityManager->storeStatement(whileStatement);

    std::vector<WhileStatement *> *whileStatements = queryFacade->getAllWhileStatements();

    REQUIRE(whileStatements->size() == 1);
    REQUIRE(whileStatements->at(0)->equals(whileStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve if statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    entityManager->storeStatement(ifStatement);

    std::vector<IfStatement *> *ifStatements = queryFacade->getAllIfStatements();

    REQUIRE(ifStatements->size() == 1);
    REQUIRE(ifStatements->at(0)->equals(ifStatement.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve procedure") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("procedure"));
    entityManager->storeProcedure(procedure);

    std::vector<Procedure *> *procedures = queryFacade->getAllProcedures();
    REQUIRE(procedures->size() == 1);
    REQUIRE(procedures->at(0)->equals(procedure.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve variable") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeVariable(variable);

    std::vector<Variable *> *variables = queryFacade->getAllVariables();
    REQUIRE(variables->size() == 1);
    REQUIRE(variables->at(0)->equals(variable.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve constant") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Constant> constant = std::make_shared<Constant>(1);
    entityManager->storeConstant(constant);

    std::vector<Constant *> *constants = queryFacade->getAllConstants();
    REQUIRE(constants->size() == 1);
    REQUIRE(constants->at(0)->equals(constant.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFace can retrieve all statements") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(5);
    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(1);
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(2);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(3);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeStatement(ifStatement);
    entityManager->storeStatement(whileStatement);
    entityManager->storeStatement(readStatement);
    entityManager->storeVariable(variable);

    std::vector<Statement *> *statements = queryFacade->getAllStatements();
    REQUIRE(statements->size() == 3);
    REQUIRE(Entity::isStatementType(statements->at(0)->getEntityType()));
    REQUIRE(Entity::isStatementType(statements->at(1)->getEntityType()));
    REQUIRE(Entity::isStatementType(statements->at(2)->getEntityType()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve follow relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship>(ifStatement, whileStatement);
    std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_shared<FollowsRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<FollowsRelationship *> *followsRelationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, WHILE_STATEMENT);
    REQUIRE(followsRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(0), followsRelationship.get()));

    std::vector<FollowsRelationship *> *followsRelationships2 = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, READ_STATEMENT);
    REQUIRE(followsRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships2->at(0), followsRelationship2.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
    delete ifStatement;
    delete whileStatement;
    delete whileStatement2;
    delete readStatement;
}

TEST_CASE("QueryFacade can retrieve parent relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<ParentRelationship> parentRelationship = std::make_shared<ParentRelationship>(ifStatement, whileStatement);
    std::shared_ptr<ParentRelationship> parentRelationship2 = std::make_shared<ParentRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(parentRelationship);
    relationshipManager->storeRelationship(parentRelationship2);

    std::vector<ParentRelationship *> *parentRelationships = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, WHILE_STATEMENT);
    REQUIRE(parentRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(parentRelationships->at(0), parentRelationship.get()));

    std::vector<ParentRelationship *> *parentRelationships2 = queryFacade->getParentRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, READ_STATEMENT);
    REQUIRE(parentRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(parentRelationships2->at(0), parentRelationship2.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
    delete ifStatement;
    delete whileStatement;
    delete whileStatement2;
    delete readStatement;
}

TEST_CASE("QueryFacade can retrieve followstar relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<FollowsStarRelationship> followsStarRelationship = std::make_shared<FollowsStarRelationship>(ifStatement, whileStatement);
    std::shared_ptr<FollowsStarRelationship> followsStarRelationship2 = std::make_shared<FollowsStarRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<FollowsStarRelationship> followsStarRelationship3 = std::make_shared<FollowsStarRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsStarRelationship);
    relationshipManager->storeRelationship(followsStarRelationship2);
    relationshipManager->storeRelationship(followsStarRelationship3);

    std::vector<FollowsStarRelationship *> *followsStarRelationships = queryFacade->getFollowsStarRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, READ_STATEMENT);
    REQUIRE(followsStarRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsStarRelationships->at(0), followsStarRelationship3.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
    delete ifStatement;
    delete whileStatement;
    delete whileStatement2;
    delete readStatement;
}

TEST_CASE("QueryFacade can retrieve ParentStar relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<ParentStarRelationship> parentStarRelationship = std::make_shared<ParentStarRelationship>(ifStatement, readStatement);
    std::shared_ptr<ParentStarRelationship> parentStarRelationship2 = std::make_shared<ParentStarRelationship>(whileStatement, whileStatement2);
    std::shared_ptr<ParentStarRelationship> parentStarRelationship3 = std::make_shared<ParentStarRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(parentStarRelationship);
    relationshipManager->storeRelationship(parentStarRelationship2);
    relationshipManager->storeRelationship(parentStarRelationship3);

    std::vector<ParentStarRelationship *> *parentStarRelationships = queryFacade->getParentStarRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, READ_STATEMENT);
    REQUIRE(parentStarRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(parentStarRelationships->at(0), parentStarRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
    delete ifStatement;
    delete whileStatement;
    delete whileStatement2;
    delete readStatement;
}

TEST_CASE("QueryFacade can retrieve modifies relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(1);
    Variable *variable = new Variable(new std::string("variable1"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    std::shared_ptr<ModifiesRelationship> modifiesRelationship = std::make_shared<ModifiesRelationship>(ifStatement, variable);
    std::shared_ptr<ModifiesRelationship> modifiesRelationship2 = std::make_shared<ModifiesRelationship>(whileStatement, variable2);

    relationshipManager->storeRelationship(modifiesRelationship);
    relationshipManager->storeRelationship(modifiesRelationship2);

    std::vector<ModifiesRelationship *> *modifiesRelationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(modifiesRelationships->at(0), modifiesRelationship.get()));

    std::vector<ModifiesRelationship *> *modifiesRelationships2 = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(modifiesRelationships2->at(0), modifiesRelationship2.get()));

    delete ifStatement;
    delete whileStatement;
    delete variable;
    delete variable2;
    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve uses relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);
    Variable *variable = new Variable(new std::string("variable"));
    Variable *variable2 = new Variable(new std::string("variable2"));

    std::shared_ptr<UsesRelationship> usesRelationship = std::make_shared<UsesRelationship>(ifStatement, variable);
    std::shared_ptr<UsesRelationship> usesRelationship2 = std::make_shared<UsesRelationship>(whileStatement, variable2);
    relationshipManager->storeRelationship(usesRelationship);
    relationshipManager->storeRelationship(usesRelationship2);

    std::vector<UsesRelationship *> *usesRelationships = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
    REQUIRE(usesRelationships->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(usesRelationships->at(0), usesRelationship.get()));

    std::vector<UsesRelationship *> *usesRelationships2 = queryFacade->getUsesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
    REQUIRE(usesRelationships2->size() == 1);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(usesRelationships2->at(0), usesRelationship2.get()));

    delete ifStatement;
    delete whileStatement;
    delete variable;
    delete variable2;
    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade gives empty vector") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::vector<ModifiesRelationship *> *modifiesRelationships = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(IF_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships->size() == 0);

    std::vector<ModifiesRelationship *> *modifiesRelationships2 = queryFacade->getModifiesRelationshipsByLeftAndRightEntityTypes(WHILE_STATEMENT, VARIABLE);
    REQUIRE(modifiesRelationships2->size() == 0);

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement Follows Statement relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    IfStatement *ifStatement = new IfStatement(1);
    WhileStatement *whileStatement = new WhileStatement(2);

    WhileStatement *whileStatement2 = new WhileStatement(3);
    ReadStatement *readStatement = new ReadStatement(4);

    std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship>(ifStatement, whileStatement);
    std::shared_ptr<FollowsRelationship> followsRelationship2 = std::make_shared<FollowsRelationship>(whileStatement2, readStatement);

    relationshipManager->storeRelationship(followsRelationship);
    relationshipManager->storeRelationship(followsRelationship2);

    std::vector<FollowsRelationship *> *followsRelationships = queryFacade->getFollowsRelationshipsByLeftAndRightEntityTypes(STATEMENT, STATEMENT);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(0), followsRelationship.get()));
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(followsRelationships->at(1), followsRelationship2.get()));

    delete ifStatement;
    delete whileStatement;
    delete whileStatement2;
    delete readStatement;
    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement Modifies Variable relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeStatement(ifStatement);
    entityManager->storeVariable(variable);

    std::shared_ptr<ModifiesRelationship> modifiesRelationship = std::make_shared<ModifiesRelationship>(ifStatement.get(), variable.get());

    relationshipManager->storeRelationship(modifiesRelationship);
    REQUIRE(queryFacade->getStatementModifiesVariableRelationship(1, "variable") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getStatementModifiesVariableRelationship(1, "variable"), modifiesRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement Uses Variable relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeStatement(ifStatement);
    entityManager->storeVariable(variable);

    std::shared_ptr<UsesRelationship> usesRelationship = std::make_shared<UsesRelationship>(ifStatement.get(), variable.get());

    relationshipManager->storeRelationship(usesRelationship);
    REQUIRE(queryFacade->getStatementUsesVariableRelationship(1, "variable") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getStatementUsesVariableRelationship(1, "variable"), usesRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Procedure Modifies Variable relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("procedure"));
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeProcedure(procedure);
    entityManager->storeVariable(variable);

    std::shared_ptr<ModifiesRelationship> modifiesRelationship = std::make_shared<ModifiesRelationship>(procedure.get(), variable.get());

    relationshipManager->storeRelationship(modifiesRelationship);
    REQUIRE(queryFacade->getProcedureModifiesVariableRelationship("procedure", "variable") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getProcedureModifiesVariableRelationship("procedure", "variable"), modifiesRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Procedure Uses Variable relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("procedure"));
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeProcedure(procedure);
    entityManager->storeVariable(variable);

    std::shared_ptr<UsesRelationship> usesRelationship = std::make_shared<UsesRelationship>(procedure.get(), variable.get());

    relationshipManager->storeRelationship(usesRelationship);
    REQUIRE(queryFacade->getProcedureUsesVariableRelationship("procedure", "variable") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getProcedureUsesVariableRelationship("procedure", "variable"), usesRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement Follows Statement relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(2);
    entityManager->storeStatement(ifStatement);
    entityManager->storeStatement(whileStatement);

    std::shared_ptr<FollowsRelationship> followsRelationship = std::make_shared<FollowsRelationship>(ifStatement.get(), whileStatement.get());

    relationshipManager->storeRelationship(followsRelationship);
    REQUIRE(queryFacade->getFollowsRelationship(1, 2) != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getFollowsRelationship(1, 2), followsRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement FollowsStar Statement relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(2);
    entityManager->storeStatement(ifStatement);
    entityManager->storeStatement(whileStatement);

    std::shared_ptr<FollowsStarRelationship> followsStarRelationship = std::make_shared<FollowsStarRelationship>(ifStatement.get(), whileStatement.get());

    relationshipManager->storeRelationship(followsStarRelationship);
    REQUIRE(queryFacade->getFollowsStarRelationship(1, 2) != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getFollowsStarRelationship(1, 2), followsStarRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement Parent Statement relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(2);
    entityManager->storeStatement(ifStatement);
    entityManager->storeStatement(whileStatement);

    std::shared_ptr<ParentRelationship> parentRelationship = std::make_shared<ParentRelationship>(ifStatement.get(), whileStatement.get());

    relationshipManager->storeRelationship(parentRelationship);
    REQUIRE(queryFacade->getParentRelationship(1, 2) != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getParentRelationship(1, 2), parentRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Statement ParentStar Statement relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>(1);
    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(2);
    entityManager->storeStatement(ifStatement);
    entityManager->storeStatement(whileStatement);

    std::shared_ptr<ParentStarRelationship> parentStarRelationship = std::make_shared<ParentStarRelationship>(ifStatement.get(), whileStatement.get());

    relationshipManager->storeRelationship(parentStarRelationship);
    REQUIRE(queryFacade->getParentStarRelationship(1, 2) != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getParentStarRelationship(1, 2), parentStarRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Caller Calls Callee relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string("caller"));
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string("callee"));
    entityManager->storeProcedure(caller);
    entityManager->storeProcedure(callee);

    std::shared_ptr<CallsRelationship> callsRelationship = std::make_shared<CallsRelationship>(caller.get(), callee.get());

    relationshipManager->storeRelationship(callsRelationship);
    REQUIRE(queryFacade->getCallsRelationship("caller", "callee") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getCallsRelationship("caller", "callee"), callsRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve Caller CallsStar Callee relationship by literals") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string("caller"));
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string("callee"));
    entityManager->storeProcedure(caller);
    entityManager->storeProcedure(callee);

    std::shared_ptr<CallsStarRelationship> callsStarRelationship = std::make_shared<CallsStarRelationship>(caller.get(), callee.get());

    relationshipManager->storeRelationship(callsStarRelationship);
    REQUIRE(queryFacade->getCallsStarRelationship("caller", "callee") != nullptr);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getCallsStarRelationship("caller", "callee"), callsStarRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve all Calls Relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string("caller"));
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string("callee"));

    std::shared_ptr<Procedure> caller2 = std::make_shared<Procedure>(new std::string("caller2"));
    std::shared_ptr<Procedure> callee2 = std::make_shared<Procedure>(new std::string("callee2"));

    entityManager->storeProcedure(caller);
    entityManager->storeProcedure(callee);
    entityManager->storeProcedure(caller2);
    entityManager->storeProcedure(callee2);

    std::shared_ptr<CallsRelationship> callsRelationship = std::make_shared<CallsRelationship>(caller.get(), callee.get());
    std::shared_ptr<CallsRelationship> callsRelationship2 = std::make_shared<CallsRelationship>(caller2.get(), callee2.get());

    relationshipManager->storeRelationship(callsRelationship);
    relationshipManager->storeRelationship(callsRelationship2);

    REQUIRE(queryFacade->getAllCallsRelationships()->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getAllCallsRelationships()->at(0), callsRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("QueryFacade can retrieve all CallsStar Relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    QueryFacade *queryFacade = new QueryFacade(entityManager, relationshipManager);

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string("caller"));
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string("callee"));

    std::shared_ptr<Procedure> caller2 = std::make_shared<Procedure>(new std::string("caller2"));
    std::shared_ptr<Procedure> callee2 = std::make_shared<Procedure>(new std::string("callee2"));

    entityManager->storeProcedure(caller);
    entityManager->storeProcedure(callee);
    entityManager->storeProcedure(caller2);
    entityManager->storeProcedure(callee2);

    std::shared_ptr<CallsStarRelationship> callsStarRelationship = std::make_shared<CallsStarRelationship>(caller.get(), callee.get());
    std::shared_ptr<CallsStarRelationship> callsStarRelationship2 = std::make_shared<CallsStarRelationship>(caller2.get(), callee2.get());

    relationshipManager->storeRelationship(callsStarRelationship);
    relationshipManager->storeRelationship(callsStarRelationship2);

    REQUIRE(queryFacade->getAllCallsStarRelationships()->size() == 2);
    REQUIRE(PKBtestHelpers::relationshipEqualsRelationship(queryFacade->getAllCallsStarRelationships()->at(0), callsStarRelationship.get()));

    delete queryFacade;
    delete entityManager;
    delete relationshipManager;
}
