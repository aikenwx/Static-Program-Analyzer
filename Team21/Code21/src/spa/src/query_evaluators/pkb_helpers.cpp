//
// Created by Dhruv Shah on 18/3/23.
//

#include "pkb_helpers.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"

namespace qps {

auto DesignEntityToEntityType(DesignEntity entity) -> EntityType {
  switch (entity) {
    case DesignEntity::STMT:return Statement::getEntityTypeStatic();
    case DesignEntity::READ:return ReadStatement::getEntityTypeStatic();
    case DesignEntity::PRINT:return PrintStatement::getEntityTypeStatic();
    case DesignEntity::CALL:return CallStatement::getEntityTypeStatic();
    case DesignEntity::WHILE:return WhileStatement::getEntityTypeStatic();
    case DesignEntity::IF:return IfStatement::getEntityTypeStatic();
    case DesignEntity::ASSIGN:return AssignStatement::getEntityTypeStatic();
    case DesignEntity::VARIABLE:return Variable::getEntityTypeStatic();
    case DesignEntity::CONSTANT:return Constant::getEntityTypeStatic();
    case DesignEntity::PROCEDURE:return Procedure::getEntityTypeStatic();
  }
}

auto GetEntityType(Synonym &syn, std::vector<Declaration> &declarations) -> EntityType {
  auto decl = Declaration::findDeclarationWithSynonym(declarations, syn);
  if (!decl) {
    throw std::invalid_argument("Synonym in clause not in query declaration");
  }
  return DesignEntityToEntityType(decl->getDesignEntity());

}

auto GetStatement(int stmt_no, QueryFacade &pkb) -> Statement * {
  return dynamic_cast<Statement *>(pkb.getEntity(Statement::getEntityTypeStatic(), stmt_no));
}

auto GetStmtNo(Entity *entity) -> int {
  return dynamic_cast<Statement *>(entity)->getStatementNumber();
}

auto GetEntities(Synonym &syn, std::vector<Declaration> &declarations, QueryFacade &pkb) -> std::vector<Entity *> * {
  auto entity_type = GetEntityType(syn, declarations);
  return pkb.getEntitiesByType(entity_type);
}

auto MatchesEntityType(Entity *entity, EntityType type) -> bool {
  if (type == Statement::getEntityTypeStatic()) {
    return Statement::isStatement(entity);
  }
  return entity->getEntityType() == type;
}

} // qps
