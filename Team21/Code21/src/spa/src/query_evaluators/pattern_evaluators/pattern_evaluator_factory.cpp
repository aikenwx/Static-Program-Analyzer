#include "pattern_evaluator_factory.h"
#include "assign_evaluator.h"
#include "if_evaluator.h"
#include "while_evaluator.h"
#include "./query/declaration.h"

#include "query/query_exceptions.h"

namespace qps {
auto PatternEvaluatorFactory::Create(PatternClause &clause,
                                      std::vector<Declaration> &decl_lst) -> std::unique_ptr<PatternEvaluator> {
  auto syn = clause.getStmtSynonym();
  DesignEntity des = Declaration::findDeclarationWithSynonym(decl_lst, syn).value().getDesignEntity();
  switch (des) {
    case DesignEntity::ASSIGN:return std::make_unique<AssignEvaluator>(clause, decl_lst);
    case DesignEntity::WHILE:return std::make_unique<WhileEvaluator>(clause, decl_lst);
    case DesignEntity::IF:return std::make_unique<IfEvaluator>(clause, decl_lst);
    default:throw QueryException(ErrorType::Semantic, "Semantic error. Invalid type for pattern synonym");
  }
}
}  // namespace qps
