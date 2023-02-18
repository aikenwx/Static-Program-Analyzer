#include "pattern_evaluator_factory.h"
#include "assign_evaluator.h"
#include "./query/declaration.h"

#include "query/query_exceptions.h"

namespace qps {
    std::unique_ptr<PatternEvaluator> PatternEvaluatorFactory::Create(PatternClause& clause,
        std::vector<Declaration>& decl_lst) {
        DesignEntity d = Declaration::findDeclarationWithSynonym(decl_lst, clause.getAssign()).value().getDesignEntity();
        switch (d) {
        case DesignEntity::ASSIGN:return std::make_unique<AssignEvaluator>(clause, decl_lst);
        case DesignEntity::WHILE:
        case DesignEntity::IF:
        default:throw QueryException(ErrorType::Syntactic, "Syntactic error. Invalid type for pattern synonym");
        }
    }
} // qps
