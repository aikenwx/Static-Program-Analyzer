//
// Created by Aiken Wong on 3/2/23.
//
#include "EntityFactory.h"
#include "EntityClasses/AssignStatement.h"
#include "EntityClasses/CallStatement.h"
#include "EntityClasses/IfStatement.h"
#include "EntityClasses/WhileStatement.h"
#include "EntityClasses/ReadStatement.h"
#include "EntityClasses/PrintStatement.h"
#include "EntityClasses/Constant.h"
#include "EntityClasses/Procedure.h"
#include "EntityClasses/Variable.h"


Entity *EntityFactory::createEntity(EntityType entityType, std::string entityValue) {

    checkForValidInput(entityType, InputType::STRING);

    switch (entityType) {
        case EntityType::PROCEDURE:
            return new Procedure(entityValue);
        case EntityType::VARIABLE:
            return new Variable(entityValue);
        default:
            throw(std::invalid_argument("Invalid entity type"));
    }
}

Entity *EntityFactory::createEntity(EntityType entityType, int entityValue) {

    checkForValidInput(entityType, InputType::INTEGER);

    switch (entityType) {
        case EntityType::ASSIGN_STATEMENT:
            return new AssignStatement(entityValue);
        case EntityType::CALL_STATEMENT:
            return new CallStatement(entityValue);
        case EntityType::IF_STATEMENT:
            return new IfStatement(entityValue);
        case EntityType::PRINT_STATEMENT:
            return new PrintStatement(entityValue);
        case EntityType::READ_STATEMENT:
            return new ReadStatement(entityValue);
        case EntityType::WHILE_STATEMENT:
            return new WhileStatement(entityValue);
        case EntityType::CONSTANT:
            return new Constant(entityValue);
        default:
            throw(std::invalid_argument("Invalid entity type"));
    }
}

bool EntityFactory::isStatement(EntityType entityType) {
    return entityType >= EntityType::ASSIGN_STATEMENT && entityType <= EntityType::WHILE_STATEMENT;
}

void EntityFactory::checkForValidInput(EntityType entityType, InputType inputType) {
    if (isStatement(entityType) && inputType != InputType::INTEGER) {
        throw(std::invalid_argument("Statement entity type requires integer input"));
    } else if (entityType == EntityType::CONSTANT && inputType != InputType::INTEGER) {
        throw(std::invalid_argument("Constant entity type requires integer input"));
    } else if (entityType == EntityType::PROCEDURE && inputType != InputType::STRING) {
        throw(std::invalid_argument("Procedure entity type requires string input"));
    } else if (entityType == EntityType::VARIABLE && inputType != InputType::STRING) {
        throw(std::invalid_argument("Variable entity type requires string input"));
    }
}

EntityFactory::EntityFactory() {}

