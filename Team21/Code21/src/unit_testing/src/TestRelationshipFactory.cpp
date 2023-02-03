#include "PKBStorageClasses/RelationshipFactory.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"


#include "catch.hpp"
using namespace std;

TEST_CASE("RelationshipFactory Instantiates") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    delete relationshipFactory;
}

TEST_CASE("RelationshipFactory can create Parent relationship") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    WhileStatement* whileStatement = new WhileStatement(4);
    ReadStatement* readStatement = new ReadStatement(3);

    Relationship* relationship = relationshipFactory->createRelationship(RelationshipType::PARENT, whileStatement, readStatement);

    REQUIRE(dynamic_cast<ParentRelationship*>(relationship) != nullptr);
    REQUIRE(relationship->containsEntityOnLeftHand(whileStatement));
    REQUIRE(relationship->containsEntityOnRightHand(readStatement));

    delete relationship;
    delete relationshipFactory;
    delete whileStatement;
    delete readStatement;
}

TEST_CASE("RelationshipFactory can create Uses relationship") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    Procedure* procedure = new Procedure("procedure");
    Variable* variable = new Variable("variable");

    Relationship* relationship = relationshipFactory->createRelationship(RelationshipType::USES, procedure, variable);

    REQUIRE(dynamic_cast<UsesRelationship*>(relationship) != nullptr);
    REQUIRE(relationship->containsEntityOnLeftHand(procedure));
    REQUIRE(relationship->containsEntityOnRightHand(variable));

    delete relationship;
    delete relationshipFactory;
    delete procedure;
    delete variable;
}

TEST_CASE("RelationshipFactory can create Modifies relationship") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    Procedure* procedure = new Procedure("procedure");
    Variable* variable = new Variable("variable");

    Relationship* relationship = relationshipFactory->createRelationship(RelationshipType::MODIFIES, procedure, variable);

    REQUIRE(dynamic_cast<ModifiesRelationship*>(relationship) != nullptr);
    REQUIRE(relationship->containsEntityOnLeftHand(procedure));
    REQUIRE(relationship->containsEntityOnRightHand(variable));

    delete relationship;
    delete relationshipFactory;
    delete procedure;
    delete variable;
}

TEST_CASE("RelationshipFactory can create Follows relationship") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    PrintStatement* printStatement = new PrintStatement(2);
    ReadStatement* readStatement = new ReadStatement(3);

    Relationship* relationship = relationshipFactory->createRelationship(RelationshipType::FOLLOWS, printStatement, readStatement);

    REQUIRE(dynamic_cast<FollowsRelationship*>(relationship) != nullptr);
    REQUIRE(relationship->containsEntityOnLeftHand(printStatement));
    REQUIRE(relationship->containsEntityOnRightHand(readStatement));

    delete relationship;
    delete relationshipFactory;
    delete printStatement;
    delete readStatement;
}

TEST_CASE("Parent relationship with non statement entities") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    Variable* variable = new Variable("variable");
    Constant* constant = new Constant(1);

    REQUIRE_THROWS(relationshipFactory->createRelationship(RelationshipType::PARENT, variable, constant));
    
    delete relationshipFactory;
    delete constant;
    delete variable;
}

TEST_CASE("Uses relationship with a non variable as second entity") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    AssignStatement* assignStatement = new AssignStatement(1);
    Constant* constant = new Constant(1);

    REQUIRE_THROWS(relationshipFactory->createRelationship(RelationshipType::USES, assignStatement, constant));

    delete relationshipFactory;
    delete constant;
    delete assignStatement;
}

TEST_CASE("Modifies relationship with a non variable as second entity") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    AssignStatement* assignStatement = new AssignStatement(1);
    Constant* constant = new Constant(1);

    REQUIRE_THROWS(relationshipFactory->createRelationship(RelationshipType::MODIFIES, assignStatement, constant));

    delete relationshipFactory;
    delete constant;
    delete assignStatement;
}

TEST_CASE("Follows relationship with non statement entities") {
    RelationshipFactory* relationshipFactory = new RelationshipFactory();
    Variable* variable = new Variable("variable");
    Constant* constant = new Constant(1);

    REQUIRE_THROWS(relationshipFactory->createRelationship(RelationshipType::FOLLOWS, variable, constant));

    delete relationshipFactory;
    delete constant;
    delete variable;
}
