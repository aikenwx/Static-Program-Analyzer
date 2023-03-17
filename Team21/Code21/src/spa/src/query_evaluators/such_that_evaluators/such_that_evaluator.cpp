#include "such_that_evaluator.h"

namespace qps {
EntityType GetEntityType(Synonym &syn, std::vector<Declaration> &declarations) {
  auto decl = Declaration::findDeclarationWithSynonym(declarations, syn);
  if (!decl) {
    throw std::invalid_argument("Synonym in clause not in query declaration");
  }
  return ClauseEvaluator::DesignEntityToEntityType(decl->getDesignEntity());

}
} // qps
