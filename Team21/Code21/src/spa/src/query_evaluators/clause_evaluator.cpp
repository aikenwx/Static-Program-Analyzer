#include "clause_evaluator.h"

namespace qps {

EntityType ClauseEvaluator::DesignEntityToEntityType(DesignEntity entity) {
    switch (entity) {
        case DesignEntity::STMT:
            return Statement::getEntityTypeStatic();
        case DesignEntity::READ:
            return ReadStatement::getEntityTypeStatic();
        case DesignEntity::PRINT:
            return PrintStatement::getEntityTypeStatic();
        case DesignEntity::CALL:
            return CallStatement::getEntityTypeStatic();
        case DesignEntity::WHILE:
            return WhileStatement::getEntityTypeStatic();
        case DesignEntity::IF:
            return IfStatement::getEntityTypeStatic();
        case DesignEntity::ASSIGN:
            return AssignStatement::getEntityTypeStatic();
        case DesignEntity::VARIABLE:
            return Variable::getEntityTypeStatic();
        case DesignEntity::CONSTANT:
            return Constant::getEntityTypeStatic();
        case DesignEntity::PROCEDURE:
            return Procedure::getEntityTypeStatic();
    }
}

}  // namespace qps
