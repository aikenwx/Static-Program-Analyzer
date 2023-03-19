#pragma once

#include <string>
#include <cassert>


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
  class AttributeReferenceEvaluator {
    public:
      static auto GetVarName(ReadStatement *read, QueryFacade &pkb) -> std::string;

      static auto GetVarName(PrintStatement *print, QueryFacade &pkb) -> std::string;

      static auto EvaluateAttrRef(Entity *entity, AttrName attr_name, QueryFacade &pkb) -> std::string;
  };
}  // namespace qps
