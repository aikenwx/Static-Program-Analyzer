
#include "PKB/EntityManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "catch.hpp"

TEST_CASE("EntityManager Instantiates") {
    EntityManager *entityManager = new EntityManager();
    delete entityManager;
}

TEST_CASE("EntityManager can store entity") {
    EntityManager *entityManager = new EntityManager();

    entityManager->storeEntity(new ReadStatement(1));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve entity") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);

    entityManager->storeEntity(new ReadStatement(1));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve mulitple entries") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);

    entityManager->storeEntity(new ReadStatement(1));
    entityManager->storeEntity(new ReadStatement(2));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1.get()));
    REQUIRE(entities->at(1)->equals(readStatement2.get()));

    delete entityManager;
}

TEST_CASE("EntityManager returns empty vector if no entries") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 0);

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve multiple types") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(2);

    entityManager->storeEntity(new ReadStatement(1));
    entityManager->storeEntity(new AssignStatement(2));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement.get()));

    entities = entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(assignStatement.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve multiple entities of multiple types") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);
    std::shared_ptr<AssignStatement> assignStatement1 = std::make_shared<AssignStatement>(3);
    std::shared_ptr<AssignStatement> assignStatement2 = std::make_shared<AssignStatement>(4);

    entityManager->storeEntity(new ReadStatement(1));
    entityManager->storeEntity(new ReadStatement(2));
    entityManager->storeEntity(new AssignStatement(3));
    entityManager->storeEntity(new AssignStatement(4));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(readStatement1.get()));
    REQUIRE(entities->at(1)->equals(readStatement2.get()));

    entities = entityManager->getEntitiesByType(AssignStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 2);
    REQUIRE(entities->at(0)->equals(assignStatement1.get()));
    REQUIRE(entities->at(1)->equals(assignStatement2.get()));

    delete entityManager;
}

TEST_CASE("EntityManager can retrieve all Statements") {
    // store all statements
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement1 = std::make_shared<ReadStatement>(1);
    std::shared_ptr<ReadStatement> readStatement2 = std::make_shared<ReadStatement>(2);
    std::shared_ptr<PrintStatement> printStatement1 = std::make_shared<PrintStatement>(3);
    std::shared_ptr<PrintStatement> printStatement2 = std::make_shared<PrintStatement>(4);
    std::shared_ptr<AssignStatement> assignStatement1 = std::make_shared<AssignStatement>(5);
    std::shared_ptr<AssignStatement> assignStatement2 = std::make_shared<AssignStatement>(6);
    std::shared_ptr<CallStatement> callStatement1 = std::make_shared<CallStatement>(7);
    std::shared_ptr<CallStatement> callStatement2 = std::make_shared<CallStatement>(8);
    std::shared_ptr<WhileStatement> whileStatement1 = std::make_shared<WhileStatement>(9);
    std::shared_ptr<WhileStatement> whileStatement2 = std::make_shared<WhileStatement>(10);
    std::shared_ptr<IfStatement> ifStatement1 = std::make_shared<IfStatement>(11);
    std::shared_ptr<IfStatement> ifStatement2 = std::make_shared<IfStatement>(12);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("x"));
    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("main"));

    entityManager->storeEntity(new ReadStatement(1));
    entityManager->storeEntity(new ReadStatement(2));
    entityManager->storeEntity(new PrintStatement(3));
    entityManager->storeEntity(new PrintStatement(4));
    entityManager->storeEntity(new AssignStatement(5));
    entityManager->storeEntity(new AssignStatement(6));
    entityManager->storeEntity(new CallStatement(7));
    entityManager->storeEntity(new CallStatement(8));
    entityManager->storeEntity(new WhileStatement(9));
    entityManager->storeEntity(new WhileStatement(10));
    entityManager->storeEntity(new IfStatement(11));
    entityManager->storeEntity(new IfStatement(12));
    entityManager->storeEntity(new Variable(new std::string("x")));
    entityManager->storeEntity(new Procedure(new std::string("main")));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(Statement::getEntityTypeStatic());

    REQUIRE(entities->size() == 12);

    for (Entity *entity : *entities) {
        bool isStatement = entity->getEntityType() == ReadStatement::getEntityTypeStatic() ||
                           entity->getEntityType() == PrintStatement::getEntityTypeStatic() ||
                           entity->getEntityType() == AssignStatement::getEntityTypeStatic() ||
                           entity->getEntityType() == CallStatement::getEntityTypeStatic() ||
                           entity->getEntityType() == WhileStatement::getEntityTypeStatic() ||
                           entity->getEntityType() == IfStatement::getEntityTypeStatic();

        REQUIRE(isStatement);
    }

    delete entityManager;
}

TEST_CASE("Retrieving statements from empty EntityManager returns empty vector") {
    EntityManager *entityManager = new EntityManager();

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(Statement::getEntityTypeStatic());

    REQUIRE(entities->size() == 0);

    delete entityManager;
}

TEST_CASE("Storing duplicate entity handled correctly") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);

    entityManager->storeEntity(new ReadStatement(1));
    entityManager->storeEntity(new ReadStatement(1));

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(ReadStatement::getEntityTypeStatic());

    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->equals(readStatement.get()));

    delete entityManager;
}

TEST_CASE("Can store and retrieve constant by constant value") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Constant> constant = std::make_shared<Constant>(1);

    entityManager->storeEntity(new Constant(1));

    auto key = EntityKey(&Constant::getEntityTypeStatic(), 1);

    Constant *retrievedConstant = (Constant *)entityManager->getEntity(key);

    REQUIRE(retrievedConstant->equals(constant.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve variable by variable name") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("x"));

    entityManager->storeEntity(new Variable(new std::string("x")));

    std::string word = "x";
    auto key = EntityKey(&Variable::getEntityTypeStatic(), &word);

    Variable *retrievedVariable = (Variable *)entityManager->getEntity(key);

    REQUIRE(retrievedVariable->equals(variable.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve procedure by procedure name") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string("main"));
    entityManager->storeEntity(new Procedure(new std::string("main")));

    std::string word = "main";
    auto key = EntityKey(&Procedure::getEntityTypeStatic(), &word);

    Procedure *retrievedProcedure = (Procedure *)entityManager->getEntity(key);

    REQUIRE(retrievedProcedure->equals(procedure.get()));
    delete entityManager;
}

TEST_CASE("Can store and retrieve statement by statement number") {
    EntityManager *entityManager = new EntityManager();
    std::shared_ptr<ReadStatement> readStatement = std::make_shared<ReadStatement>(1);
    entityManager->storeEntity(new ReadStatement(1));

    auto key = EntityKey(&Statement::getEntityTypeStatic(), 1);

    ReadStatement *retrievedReadStatement = (ReadStatement *)entityManager->getEntity(key);

    REQUIRE(retrievedReadStatement->equals(readStatement.get()));
    delete entityManager;
}
