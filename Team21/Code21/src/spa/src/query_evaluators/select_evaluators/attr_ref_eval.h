#pragma once

#include <string>

#include "query/attr_ref.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Statement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"

#include "PKB/QueryFacade.h"

namespace qps {
std::string GetVarName(ReadStatement *read, QueryFacade &pkb) {
  auto entities =
      pkb.getRelationshipsByLeftEntityLiteralAndRightEntityType(ModifiesRelationship::getRelationshipTypeStatic(),
                                                                read->getEntityType(), read->getStatementNumber(),
                                                                Variable::getEntityTypeStatic());
  assert(entities->size() == 1);
  return *(*entities)[0]->getEntityValue();
}

std::string GetVarName(PrintStatement *print, QueryFacade &pkb) {
  auto entities =
      pkb.getRelationshipsByLeftEntityLiteralAndRightEntityType(UsesRelationship::getRelationshipTypeStatic(),
                                                                print->getEntityType(), print->getStatementNumber(),
                                                                Variable::getEntityTypeStatic());
  assert(entities->size() == 1);
  return *(*entities)[0]->getEntityValue();
}

std::string EvaluateAttrRef(Entity *entity, AttrName attr_name, QueryFacade &pkb) {
  switch (attr_name) {
    case AttrName::ProcName:
      if (entity->getEntityType() == CallStatement::getEntityTypeStatic()) {
        auto *call_statement = dynamic_cast<CallStatement *>(entity);
        return *call_statement->getProcedureName();
      }
    case AttrName::VarName:
      if (entity->getEntityType() == PrintStatement::getEntityTypeStatic()) {
        auto *print_statement = dynamic_cast<PrintStatement *>(entity);
        return GetVarName(print_statement, pkb);
      } else if (entity->getEntityType() == ReadStatement::getEntityTypeStatic()) {
        auto *read_statement = dynamic_cast<ReadStatement *>(entity);
        return GetVarName(read_statement, pkb);
      }
    default:return *entity->getEntityValue();
  }
}
}
