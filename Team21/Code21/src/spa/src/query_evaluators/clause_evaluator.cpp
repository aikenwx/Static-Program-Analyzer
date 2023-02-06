#include "clause_evaluator.h"

namespace qps {

EntityType ClauseEvaluator::design_entity_to_entity_type(DesignEntity entity) {
  switch (entity) {
    case DesignEntity::STMT:return EntityType::STATEMENT;
    case DesignEntity::READ:return EntityType::READ_STATEMENT;
    case DesignEntity::PRINT:return EntityType::PRINT_STATEMENT;
    case DesignEntity::CALL:return EntityType::CALL_STATEMENT;
    case DesignEntity::WHILE:return EntityType::WHILE_STATEMENT;
    case DesignEntity::IF:return EntityType::IF_STATEMENT;
    case DesignEntity::ASSIGN:return EntityType::ASSIGN_STATEMENT;
    case DesignEntity::VARIABLE:return EntityType::VARIABLE;
    case DesignEntity::CONSTANT:return EntityType::CONSTANT;
    case DesignEntity::PROCEDURE:return EntityType::PROCEDURE;
  }
}
} // qps
