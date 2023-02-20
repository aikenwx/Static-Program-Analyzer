//
// Created by Aiken Wong on 3/2/23.
//

#ifndef SPA_ENTITYMANAGER_H
#define SPA_ENTITYMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "../PKBStorageClasses/EntityClasses/CallStatement.h"
#include "../PKBStorageClasses/EntityClasses/Constant.h"
#include "../PKBStorageClasses/EntityClasses/Entity.h"
#include "../PKBStorageClasses/EntityClasses/IfStatement.h"
#include "../PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "../PKBStorageClasses/EntityClasses/Procedure.h"
#include "../PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "../PKBStorageClasses/EntityClasses/Statement.h"
#include "../PKBStorageClasses/EntityClasses/Variable.h"
#include "../PKBStorageClasses/EntityClasses/WhileStatement.h"

class EntityManager {
   private:
    std::unordered_map<EntityType, std::shared_ptr<std::vector<Entity*>>> entityTypeToEntityStore;
    std::unordered_map<int, std::shared_ptr<Statement>> statementNumberToStatementStore;
    std::unordered_map<std::string, std::shared_ptr<Variable>> variableNameToVariableStore;
    std::unordered_map<std::string, std::shared_ptr<Procedure>> procedureNameToProcedureStore;
    std::unordered_map<int, std::shared_ptr<Constant>> constantValueToConstantStore;

   public:
    EntityManager();

    ~EntityManager();

    void storeConstant(std::shared_ptr<Constant> constant);
    void storeVariable(std::shared_ptr<Variable> variable);
    void storeProcedure(std::shared_ptr<Procedure> procedure);
    void storeStatement(std::shared_ptr<Statement> statement);

    std::vector<Entity*>* getEntitiesByType(EntityType entityType);

    Statement* getStatementByStatementNumber(int statementNumber);
    Variable* getVariableByVariableName(std::string variableName);
    Procedure* getProcedureByProcedureName(std::string procedureName);
    Constant* getConstantByConstantValue(int constantValue);

   private:
    void storeInEntityTypeToEntityStore(Entity* entity);

    void initialiseVectorForEntityTypeStoreIfIndexNotExist(EntityType entityType);

    std::vector<Entity*>* getAllStatements();

    template <typename T, typename S>

    bool checkIfEntityIsDuplicate(T hash, std::unordered_map<T, S>* entityStore);
};

#endif  // SPA_ENTITYMANAGER_H
