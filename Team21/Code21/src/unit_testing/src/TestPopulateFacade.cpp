#include "PKB/PopulateFacade.h"
#include "catch.hpp"


TEST_CASE("PopulateFacade can instantiate") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can add postfix expressions to assignment statements") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeAssignStatementPostfixExpression(1, "xy+");
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "1");
    REQUIRE(*((AssignStatement *)entityManager->getStatementByStatementNumber(1))->getPostFixExpression() == "xy+");
    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("Error thrown when trying to add non pattern expression to call statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeCallStatement(1);
    REQUIRE_THROWS(populateFacade->storeAssignStatementPostfixExpression(1, "xy+"));
    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate assign statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    std::string postfixExpression = "xy+";
    populateFacade->storeAssignmentStatement(1);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "1");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate call statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeCallStatement(2);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate while statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeWhileStatement(3);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::WHILE_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "3");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate if statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeIfStatement(4);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::IF_STATEMENT);
    REQUIRE(*entities->at(0)->getEntityValue() == "4");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate procedure") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::string procedureName = "main";
    populateFacade->storeProcedure(procedureName);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::PROCEDURE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*entities->at(0)->getEntityValue() == "main");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate variable") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    std::string variableName = "x";
    populateFacade->storeVariable(variableName);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::VARIABLE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::VARIABLE);
    REQUIRE(*entities->at(0)->getEntityValue() == "x");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFace can populate constant") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    populateFacade->storeConstant(5);
    std::vector<Entity *> *entities = entityManager->getEntitiesByType(EntityType::CONSTANT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CONSTANT);
    REQUIRE(*entities->at(0)->getEntityValue() == "5");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate follow relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate parent relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate parentStar relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::shared_ptr<WhileStatement> whileStatement = std::make_shared<WhileStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);

    entityManager->storeStatement(whileStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeParentStarRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT_STAR,
                                                                                              WHILE_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::PARENT_STAR);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate followStar relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::shared_ptr<CallStatement> callStatement = std::make_shared<CallStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);
    entityManager->storeStatement(callStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeFollowsStarRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS_STAR,
                                                                                              CALL_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS_STAR);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("Adding a relationship with a non-existent entity throws an exception") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    REQUIRE_THROWS_AS(populateFacade->storeParentRelationship(1, 2), std::exception);

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate statement modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate statement uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate statement ParentStar relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);
    entityManager->storeStatement(assignStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeParentStarRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(PARENT_STAR,
                                                                                              ASSIGN_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::PARENT_STAR);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate FollowsStar relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::shared_ptr<AssignStatement> assignStatement = std::make_shared<AssignStatement>(1);
    std::shared_ptr<PrintStatement> printStatement = std::make_shared<PrintStatement>(2);
    entityManager->storeStatement(assignStatement);
    entityManager->storeStatement(printStatement);

    populateFacade->storeFollowsStarRelationship(1, 2);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS_STAR,
                                                                                              ASSIGN_STATEMENT,
                                                                                              PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS_STAR);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate procedure modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate procedure uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

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
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate Caller Calls Callee relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::string callerName = "caller";
    std::string calleeName = "callee";

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string(callerName));
    entityManager->storeProcedure(caller);
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string(calleeName));
    entityManager->storeProcedure(callee);

    populateFacade->storeCallsRelationship(callerName, calleeName);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(CALLS, PROCEDURE,
                                                                                              PROCEDURE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::CALLS);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "caller");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "callee");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate Caller Calls* Callee relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);

    std::string callerName = "caller";
    std::string calleeName = "callee";

    std::shared_ptr<Procedure> caller = std::make_shared<Procedure>(new std::string(callerName));
    entityManager->storeProcedure(caller);
    std::shared_ptr<Procedure> callee = std::make_shared<Procedure>(new std::string(calleeName));
    entityManager->storeProcedure(callee);

    populateFacade->storeCallsStarRelationship(callerName, calleeName);

    std::vector<Relationship *> *relationships = relationshipManager->getRelationshipsByTypes(CALLS_STAR, PROCEDURE,
                                                                                              PROCEDURE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::CALLS_STAR);
    REQUIRE(*relationships->at(0)->getLeftHandEntity()->getEntityValue() == "caller");
    REQUIRE(*relationships->at(0)->getRightHandEntity()->getEntityValue() == "callee");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}

TEST_CASE("PopulateFacade can populate multiple entities and relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PatternManager *patternManager = new PatternManager();
    CFGManager *cfgManager = new CFGManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager, patternManager, cfgManager);
    std::string postFixPattern = "xy+";

    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeCallStatement(2);
    populateFacade->storeAssignmentStatement(3);
    populateFacade->storeAssignmentStatement(4);

    populateFacade->storeProcedure("Procedure");
    populateFacade->storeProcedure("Procedure2");

    populateFacade->storeParentStarRelationship(1, 2);
    populateFacade->storeFollowsStarRelationship(1, 3);
    populateFacade->storeFollowsRelationship(1, 3);
    populateFacade->storeFollowsRelationship(2, 4);
    populateFacade->storeCallsRelationship("Procedure", "Procedure2");
    populateFacade->storeCallsStarRelationship("Procedure", "Procedure2");

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

    std::vector<Relationship *> *relationships3 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::PARENT_STAR, EntityType::ASSIGN_STATEMENT, EntityType::CALL_STATEMENT);
    REQUIRE(relationships3->size() == 1);
    REQUIRE(relationships3->at(0)->getRelationshipType() == RelationshipType::PARENT_STAR);
    REQUIRE(relationships3->at(0)->getLeftHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships3->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships3->at(0)->getRightHandEntity()->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(*relationships3->at(0)->getRightHandEntity()->getEntityValue() == "2");

    std::vector<Relationship *> *relationships4 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS_STAR, EntityType::ASSIGN_STATEMENT, EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships4->size() == 1);
    REQUIRE(relationships4->at(0)->getRelationshipType() == RelationshipType::FOLLOWS_STAR);
    REQUIRE(relationships4->at(0)->getLeftHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships4->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships4->at(0)->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(*relationships4->at(0)->getRightHandEntity()->getEntityValue() == "3");

    std::vector<Relationship *> *relationships5 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::CALLS, EntityType::PROCEDURE, EntityType::PROCEDURE);

    REQUIRE(relationships5->size() == 1);
    REQUIRE(relationships5->at(0)->getRelationshipType() == RelationshipType::CALLS);
    REQUIRE(relationships5->at(0)->getLeftHandEntity()->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*relationships5->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(relationships5->at(0)->getRightHandEntity()->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*relationships5->at(0)->getRightHandEntity()->getEntityValue() == "Procedure2");

    std::vector<Relationship *> *relationships6 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::CALLS_STAR, EntityType::PROCEDURE, EntityType::PROCEDURE);

    REQUIRE(relationships6->size() == 1);
    REQUIRE(relationships6->at(0)->getRelationshipType() == RelationshipType::CALLS_STAR);
    REQUIRE(relationships6->at(0)->getLeftHandEntity()->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*relationships6->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(relationships6->at(0)->getRightHandEntity()->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(*relationships6->at(0)->getRightHandEntity()->getEntityValue() == "Procedure2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
    delete patternManager;
}
