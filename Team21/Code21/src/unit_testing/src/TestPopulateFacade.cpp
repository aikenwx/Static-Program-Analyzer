#include "PKB/PopulateFacade.h"
#include "catch.hpp"

TEST_CASE("PopulateFacade can instantiate") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate assign statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeAssignmentStatement(1);
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->at(0)->getEntityValue() == "1");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate call statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeCallStatement(2);
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(entities->at(0)->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate while statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeWhileStatement(3);
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::WHILE_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::WHILE_STATEMENT);
    REQUIRE(entities->at(0)->getEntityValue() == "3");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate if statement") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeIfStatement(4);
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::IF_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::IF_STATEMENT);
    REQUIRE(entities->at(0)->getEntityValue() == "4");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate procedure") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeProcedure("main");
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::PROCEDURE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::PROCEDURE);
    REQUIRE(entities->at(0)->getEntityValue() == "main");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate variable") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeVariable("x");
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::VARIABLE);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::VARIABLE);
    REQUIRE(entities->at(0)->getEntityValue() == "x");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFace can populate constant") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeConstant(5);
    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::CONSTANT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::CONSTANT);
    REQUIRE(entities->at(0)->getEntityValue() == "5");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate follow relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeFollowsRelationship(1, EntityType::CALL_STATEMENT, 2, EntityType::PRINT_STATEMENT);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(FOLLOWS, CALL_STATEMENT,
                                                                                                             PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate parent relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeParentRelationship(1, EntityType::WHILE_STATEMENT, 2, EntityType::PRINT_STATEMENT);

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(PARENT, WHILE_STATEMENT,
                                                                                                             PRINT_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::PARENT);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "2");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate statement modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeStatementModifiesVariableRelationship(1, EntityType::ASSIGN_STATEMENT, "variable");

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(MODIFIES,
                                                                                                             ASSIGN_STATEMENT,
                                                                                                             VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::MODIFIES);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate statement uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeStatementUsesVariableRelationship(1, EntityType::ASSIGN_STATEMENT, "variable");
    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(USES, ASSIGN_STATEMENT,
                                                                                                             VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::USES);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate procedure modifies relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeProcedureModifiesVariableRelationship("Procedure", "variable");

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(MODIFIES, PROCEDURE,
                                                                                                             VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::MODIFIES);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate procedure uses relationship") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);

    populateFacade->storeProcedureUsesVariableRelationship("Procedure", "variable");

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(USES, PROCEDURE,
                                                                                                             VARIABLE);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::USES);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "Procedure");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "variable");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}

TEST_CASE("PopulateFacade can populate multiple entities and relationships") {
    EntityManager *entityManager = new EntityManager();
    RelationshipManager *relationshipManager = new RelationshipManager();
    PopulateFacade *populateFacade = new PopulateFacade(entityManager, relationshipManager);
    populateFacade->storeAssignmentStatement(1);
    populateFacade->storeCallStatement(2);
    populateFacade->storeFollowsRelationship(1, EntityType::ASSIGN_STATEMENT, 3, EntityType::ASSIGN_STATEMENT);
    populateFacade->storeFollowsRelationship(2, EntityType::CALL_STATEMENT, 4, EntityType::ASSIGN_STATEMENT);

    std::vector<std::shared_ptr<Entity>> *entities = entityManager->getEntitiesByType(EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->size() == 1);
    REQUIRE(entities->at(0)->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(entities->at(0)->getEntityValue() == "1");

    std::vector<std::shared_ptr<Entity>> *entities2 = entityManager->getEntitiesByType(EntityType::CALL_STATEMENT);
    REQUIRE(entities2->size() == 1);
    REQUIRE(entities2->at(0)->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(entities2->at(0)->getEntityValue() == "2");

    std::vector<std::shared_ptr<Relationship>> *relationships = relationshipManager->getRelationshipsByTypes(RelationshipType::FOLLOWS,
                                                                                                             EntityType::ASSIGN_STATEMENT,
                                                                                                             EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships->size() == 1);
    REQUIRE(relationships->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships->at(0)->getLeftHandEntity()->getEntityValue() == "1");
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships->at(0)->getRightHandEntity()->getEntityValue() == "3");

    std::vector<std::shared_ptr<Relationship>> *relationships2 = relationshipManager->getRelationshipsByTypes(
        RelationshipType::FOLLOWS, EntityType::CALL_STATEMENT, EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships2->size() == 1);
    REQUIRE(relationships2->at(0)->getRelationshipType() == RelationshipType::FOLLOWS);
    REQUIRE(relationships2->at(0)->getLeftHandEntity()->getEntityType() == EntityType::CALL_STATEMENT);
    REQUIRE(relationships2->at(0)->getLeftHandEntity()->getEntityValue() == "2");
    REQUIRE(relationships2->at(0)->getRightHandEntity()->getEntityType() == EntityType::ASSIGN_STATEMENT);
    REQUIRE(relationships2->at(0)->getRightHandEntity()->getEntityValue() == "4");

    delete populateFacade;
    delete entityManager;
    delete relationshipManager;
}
