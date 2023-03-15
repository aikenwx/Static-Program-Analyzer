#pragma once

#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"
#include "PKBStorageClasses/RelationshipClasses/UsesRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/ModifiesRelationship.h"

#include <variant>

namespace qps {

	using Product = std::variant<IfStatement, WhileStatement, UsesRelationship, ModifiesRelationship>;
}
