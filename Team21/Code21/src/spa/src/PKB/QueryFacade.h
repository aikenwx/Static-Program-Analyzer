//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_QUERYFACADE_H
#define SPA_QUERYFACADE_H

#include <unordered_set>
#include <vector>

#include "PKB/CFGStorage.h"
#include "PKB/EntityManager.h"
#include "PKB/PatternManager.h"
#include "PKB/RelationshipManager.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/CallsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/CallsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/FollowsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ParentStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "sp/cfg/cfg.h"

class QueryFacade {
 private:
  EntityManager* entityManager;
  RelationshipManager* relationshipManager;
  CFGStorage* cfgManager;
  PatternManager* patternManager;

 public:
  QueryFacade(EntityManager* entityManager, RelationshipManager* relationshipManager, PatternManager* patternManager, CFGStorage* cfgManager);

  auto getAllAssignStatements() -> std::vector<AssignStatement*>*;
  auto getAllIfStatements() -> std::vector<IfStatement*>*;
  auto getAllWhileStatements() -> std::vector<WhileStatement*>*;
  auto getAllCallStatements() -> std::vector<CallStatement*>*;
  auto getAllReadStatements() -> std::vector<ReadStatement*>*;
  auto getAllPrintStatements() -> std::vector<PrintStatement*>*;
  auto getAllProcedures() -> std::vector<Procedure*>*;
  auto getAllVariables() -> std::vector<Variable*>*;
  auto getAllConstants() -> std::vector<Constant*>*;
  auto getAllStatements() -> std::vector<Statement*>*;

  auto getParentRelationshipsByLeftAndRightEntityTypes(
      const EntityType & leftEntityType, const EntityType & rightEntityType)
      -> std::vector<ParentRelationship*>*;
  auto getFollowsRelationshipsByLeftAndRightEntityTypes(
      const EntityType & leftEntityType, const EntityType & rightEntityType)
      -> std::vector<FollowsRelationship*>*;
  auto getModifiesRelationshipsByLeftAndRightEntityTypes(
      const EntityType & leftEntityType, const EntityType & rightEntityType)
      -> std::vector<ModifiesRelationship*>*;
  auto getUsesRelationshipsByLeftAndRightEntityTypes(
      const EntityType & leftEntityType, const EntityType & rightEntityType)
      -> std::vector<UsesRelationship*>*;
  auto getParentStarRelationshipsByLeftAndRightEntityTypes(
      const EntityType & leftEntityType, const EntityType & rightEntityType)
      -> std::vector<ParentStarRelationship*>*;

  auto getAllCallsRelationships() -> std::vector<CallsRelationship*>*;
  auto getAllCallsStarRelationships() -> std::vector<CallsStarRelationship*>*;

  auto getWhileStatementsUsingVariableInCondition(std::string variableName)
      -> std::unordered_set<WhileStatement*>*;
  auto getIfStatementsUsingVariableInCondition(std::string variableName)
      -> std::unordered_set<IfStatement*>*;

  auto getVariablesInWhileStatementCondition(int statementNumber)
      -> std::unordered_set<Variable*>*;

  auto getVariablesInIfStatementCondition(int statementNumber)
      -> std::unordered_set<Variable*>*;


  auto getEntity(const EntityType& entityType, int entityValue) -> Entity*;

  auto getEntity(const EntityType& entityType, std::string entityValue) -> Entity*;

  auto getEntitiesByType(const EntityType& entityType) -> std::vector<Entity*>*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, int leftEntityValue,
                       const EntityType & rightEntityType , int rightEntityValue)
      -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, std::string leftEntityValue,
                       const EntityType & rightEntityType ,
                       std::string rightEntityValue) -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, int leftEntityValue,
                       const EntityType & rightEntityType,
                       std::string rightEntityValue) -> Relationship*;

  auto getRelationship(const RelationshipType & relationshipType,
                       const EntityType & leftEntityType, std::string leftEntityValue,
                       const EntityType & rightEntityType, int rightEntityValue)
      -> Relationship*;

  auto getRelationshipsByTypes(const RelationshipType & relationshipType,
                               const EntityType & leftEntityType,
                               const EntityType & rightEntityType)
      -> std::vector<Relationship*>*;

  auto getRelationshipsByLeftEntityTypeAndRightEntityLiteral(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      const EntityType & rightEntityType, int rightEntityValue)
      -> std::vector<Entity*>*;

  auto getRelationshipsByLeftEntityTypeAndRightEntityLiteral(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      const EntityType & rightEntityType, std::string rightEntityValue)
      -> std::vector<Entity*>*;

  auto getRelationshipsByLeftEntityLiteralAndRightEntityType(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      int leftEntityValue, const EntityType & rightEntityType)
      -> std::vector<Entity*>*;

  auto getRelationshipsByLeftEntityLiteralAndRightEntityType(
      const RelationshipType & relationshipType, const EntityType & leftEntityType,
      std::string leftEntityValue, const EntityType & rightEntityType)
      -> std::vector<Entity*>*;

  auto getCFG() -> cfg::CFG*;

  void clearCache();
};

#endif  // SPA_QUERYFACADE_H
