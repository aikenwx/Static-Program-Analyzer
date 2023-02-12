#include "PKB/PopulateFacade.h"
#include "catch.hpp"

TEST_CASE("PopulateFacade can instantiate") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate assign statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    std::string postfixExpression = "xy+";
    populateFacade->storeAssignmentStatement(1);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "1");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate call statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    populateFacade->storeCallStatement(2);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate while statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    populateFacade->storeWhileStatement(3);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::WHILE_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "3");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate if statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    populateFacade->storeIfStatement(4);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::IF_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "4");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate procedure") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::string procedureName = "main";
    populateFacade->storeProcedure(procedureName);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::PROCEDURE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*entities->at(0)->getEntityValue() == "main");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate variable") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    std::string variableName = "x";
    populateFacade->storeVariable(variableName);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::VARIABLE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::VARIABLE);
    REQUIRE(*entities->at(0)->getEntityValue() == "x");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate constant") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    populateFacade->storeConstant(5);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::CONSTANT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CONSTANT);
    REQUIRE(*entities->at(0)->getEntityValue() == "5");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate follow relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::shared_ptr<CallStatement> callStatement = std::make_shared<CallStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);
    entityManager->storeStatement(callStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeFollowsRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate parent relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);

    entityManager->storeStatement(whileStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeParentRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, WHILE_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::PARENT);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("Adding a relationship with a non-existent entity throws an exception") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    REQUIRE_THROWS_AS(populateFacade->storeParentRelationship(1, 2), std::exception);

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate statement modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::string variableName = "variable";

    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(1);
    entityManager->storeStatement(assignStatement);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string("variable"));
    entityManager->storeVariable(variable);

    populateFacade->storeStatementModifiesVariableRelationship(1, variableName);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                              ASSIGN_STATEMENT,
                                                                                              VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::MODIFIES);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate statement uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    std::string variableName = "variable";

    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(1);
    entityManager->storeStatement(assignStatement);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string(variableName));
    entityManager->storeVariable(variable);
    populateFacade->storeStatementUsesVariableRelationship(1, variableName);
    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(USES, ASSIGN_STATEMENT,
                                                                                              VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::USES);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate procedure modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::string variableName = "variable";
    std::string procedureName = "Procedure";

    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string(procedureName));
    entityManager->storeProcedure(procedure);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string(variableName));
    entityManager->storeVariable(variable);
    populateFacade->storeProcedureModifiesVariableRelationship(procedureName, variableName);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(MODIFIES, PROCEDURE,
                                                                                              VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::MODIFIES);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate procedure uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);

    std::string variableName = "variable";
    std::string procedureName = "Procedure";

    std::shared_ptr<Procedure> procedure = std::make_shared<Procedure>(new std::string(procedureName));
    entityManager->storeProcedure(procedure);
    std::shared_ptr<Variable> variable = std::make_shared<Variable>(new std::string(variableName));
    entityManager->storeVariable(variable);

    populateFacade->storeProcedureUsesVariableRelationship(procedureName, variableName);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(USES, PROCEDURE,
                                                                                              VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::USES);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate multiple entities and relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager);
    std::string postFixPattern = "xy+";

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeCallStatement(2);
    populateFacade->storeAssignmentStatement(3);
    populateFacade->storeAssignmentStatement(4);

    populateFacade->storeFollowsRelationship(1, 3);
    populateFacade->storeFollowsRelationship(2, 4);

    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 3);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "1");

    std::vector<Entity *> *entities2 = entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
    REQUIRE(entities2->size() == 1);
    REQUIRE(entities2->at(0)->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(*entities2->at(0)->getEntityValue() == "2");

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(RelationshipType::FOLLOWS,
                                                                                              EntityType::ASSIGN_STATEMENT,
                                                                                              EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "3");

    std::vector<Relationship *> *relationships2 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS, EntityType::CALL_STATEMENT, EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships2->size() == 1);
    REQUIRE(relationships2->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(relationships2->at(0)->getLeftHandEntity()->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(*relationships2->at(0)->getLeftHandEntity()->getEntityValue() == "2");
    REQUIRE(relationships2->at(0)->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships2->at(0)->getRightHandEntity()->getEntityValue() == "4");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}
