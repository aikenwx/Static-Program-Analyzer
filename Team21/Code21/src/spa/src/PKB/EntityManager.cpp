//
// Created by Aiken Wong on 3/2/23.
//

#include "EntityManager.h"

#include <iostream>
#include <stdexcept>

EntityManager::EntityManager() {
    this->entityTypeToEntityStore = std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity*>>>();

    // ownership of the entities are passed to these stores
    this->statementNumberToStatementStore = std::unordered_map<int, std::shared_ptr<Statement>>();
    this->variableNameToVariableStore = std::unordered_map<std::string, std::shared_ptr<Variable>>();
    this->procedureNameToProcedureStore = std::unordered_map<std::string, std::shared_ptr<Procedure>>();
    this->constantValueToConstantStore = std::unordered_map<int, std::shared_ptr<Constant>>();
}

EntityManager::~EntityManager() {
    this->statementNumberToStatementStore.clear();

    this->entityTypeToEntityStore.clear();
    this->variableNameToVariableStore.clear();
    this->procedureNameToProcedureStore.clear();
    this->constantValueToConstantStore.clear();
}

void EntityManager::storeConstant(std::shared_ptr<Constant> constant) {
    if (this->checkIfEntityIsDuplicate(constant->getConstantNumber(), &this->constantValueToConstantStore)) {
        return;
    }

    constantValueToConstantStore.insert({constant->getConstantNumber(), std::shared_ptr<Constant>(constant)});
    this->storeInEntityTypeToEntityStore(constant.get());
}

void EntityManager::storeVariable(std::shared_ptr<Variable> variable) {
    if (this->checkIfEntityIsDuplicate(*variable->getEntityValue(), &this->variableNameToVariableStore)) {
        return;
    }

    variableNameToVariableStore.insert({*variable->getEntityValue(), std::shared_ptr<Variable>(variable)});
    this->storeInEntityTypeToEntityStore(variable.get());
}
void EntityManager::storeProcedure(std::shared_ptr<Procedure> procedure) {
    if (this->checkIfEntityIsDuplicate(*procedure->getEntityValue(), &this->procedureNameToProcedureStore)) {
        return;
    }

    procedureNameToProcedureStore.insert({*procedure->getEntityValue(), std::shared_ptr<Procedure>(procedure)});
    this->storeInEntityTypeToEntityStore(procedure.get());
}
void EntityManager::storeStatement(std::shared_ptr<Statement> statement) {
    if (this->checkIfEntityIsDuplicate(statement->getStatementNumber(), &this->statementNumberToStatementStore)) {
        return;
    }

    statementNumberToStatementStore.insert({statement->getStatementNumber(), std::shared_ptr<Statement>(statement)});
    this->storeInEntityTypeToEntityStore(statement.get());
}

void EntityManager::storeInEntityTypeToEntityStore(Entity* entity) {
    initialiseVectorForEntityTypeStoreIfIndexNotExist(entity->getEntityType());
    this->entityTypeToEntityStore.at(entity->getEntityType())->push_back(entity);
}

Statement* EntityManager::getStatementByStatementNumber(int statementNumber) {
    if (this->statementNumberToStatementStore.find(statementNumber) == this->statementNumberToStatementStore.end()) {
        return nullptr;
    }

    return this->statementNumberToStatementStore.at(statementNumber).get();
}

Constant* EntityManager::getConstantByConstantValue(int constantValue) {
    if (this->constantValueToConstantStore.find(constantValue) == this->constantValueToConstantStore.end()) {
        return nullptr;
    }

    return this->constantValueToConstantStore.at(constantValue).get();
}

Variable* EntityManager::getVariableByVariableName(std::string variableName) {
    if (this->variableNameToVariableStore.find(variableName) == this->variableNameToVariableStore.end()) {
        return nullptr;
    }

    return this->variableNameToVariableStore.at(variableName).get();
}

Procedure* EntityManager::getProcedureByProcedureName(std::string procedureName) {
    if (this->procedureNameToProcedureStore.find(procedureName) == this->procedureNameToProcedureStore.end()) {
        return nullptr;
    }

    return this->procedureNameToProcedureStore.at(procedureName).get();
}

std::vector<Entity*>* EntityManager::getEntitiesByType(EntityType entityType) {
    if (entityType == EntityType::STATEMENT) {
        return this->getAllStatements();
    }
    // initialise the vector if it does not exist
    EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(entityType);
    return this->entityTypeToEntityStore.at(entityType).get();
}

std::vector<Entity*>* EntityManager::getAllStatements() {
    if (this->entityTypeToEntityStore.find(EntityType::STATEMENT) != this->entityTypeToEntityStore.end()) {
        return this->entityTypeToEntityStore.at(EntityType::STATEMENT).get();
    }

    std::shared_ptr<std::vector<Entity*>> statementVector = std::make_shared<std::vector<Entity*>>();

    for (EntityType statementType : Entity::statementTypes) {
        EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(statementType);
        statementVector->insert(statementVector->end(), this->entityTypeToEntityStore.at(statementType)->begin(), this->entityTypeToEntityStore.at(statementType)->end());
    }
    this->entityTypeToEntityStore.insert({EntityType::STATEMENT, statementVector});
    return statementVector.get();
}

void EntityManager::initialiseVectorForEntityTypeStoreIfIndexNotExist(EntityType entityType) {
    if (this->entityTypeToEntityStore.find(entityType) == this->entityTypeToEntityStore.end()) {
        this->entityTypeToEntityStore.insert({entityType, std::make_shared<std::vector<Entity*>>()});
    }
}

template <typename T, typename S>
bool EntityManager::checkIfEntityIsDuplicate(T hash, std::unordered_map<T, S>* entityStore) {
    if (entityStore->find(hash) != entityStore->end()) {
        return true;
    }
    return false;
}
