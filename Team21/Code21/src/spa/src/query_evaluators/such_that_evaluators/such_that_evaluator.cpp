#include "such_that_evaluator.h"

#include "query/ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/pkb_helpers.h"

namespace qps {

class ClauseVisitor {
 public:
  ClauseVisitor(QueryFacade &pkb, const RelationshipType &relationship, const EntityType &left, const EntityType &right)
      : pkb_(pkb), relationship_type_(relationship), left_(left), right_(right) {}

  auto operator()(StatementNumber from, StatementNumber dest) -> ClauseResult {
    auto *relationship = pkb_.getRelationship(relationship_type_, left_, from, right_, dest);
    return relationship != nullptr;
  }

  auto operator()(StatementNumber from, QuotedIdentifier dest) -> ClauseResult {
    auto *relationship = pkb_.getRelationship(relationship_type_, left_, from, right_, dest.getQuotedId());
    return relationship != nullptr;
  }

  auto operator()(QuotedIdentifier from, StatementNumber dest) -> ClauseResult {
    auto
        *relationship = pkb_.getRelationship(relationship_type_, left_, from.getQuotedId(), right_, dest);
    return relationship != nullptr;
  }

  auto operator()(QuotedIdentifier from, QuotedIdentifier dest) -> ClauseResult {
    auto
        *relationship = pkb_.getRelationship(relationship_type_, left_, from.getQuotedId(), right_, dest.getQuotedId());
    return relationship != nullptr;
  }

  auto operator()(StatementNumber from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *entities =
        pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_, left_, from, right_);
    return !(entities->empty());
  }

  auto operator()(QuotedIdentifier from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *entities = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                                left_,
                                                                                from.getQuotedId(),
                                                                                right_);
    return !(entities->empty());
  }

  auto operator()([[maybe_unused]] Underscore underscore, StatementNumber dest) -> ClauseResult {
    auto *entities =
        pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_, left_, right_, dest);
    return !(entities->empty());
  }

  auto operator()([[maybe_unused]] Underscore underscore, QuotedIdentifier dest) -> ClauseResult {
    auto *entities = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                                left_,
                                                                                right_,
                                                                                dest.getQuotedId());
    return !(entities->empty());
  }

  auto operator()(StatementNumber src, Synonym dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                              left_,
                                                                              src,

                                                                              right_);
    return SynonymTable(std::move(dest), *entities);
  }

  auto operator()(Synonym from, StatementNumber dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                              left_,
                                                                              right_,
                                                                              dest);

    return SynonymTable(std::move(from), *entities);
  }

  auto operator()(QuotedIdentifier src, Synonym dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                              left_,
                                                                              src.getQuotedId(),
                                                                              right_);
    return SynonymTable(std::move(dest), *entities);
  }

  auto operator()(Synonym from, QuotedIdentifier dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                              left_,
                                                                              right_,
                                                                              dest.getQuotedId());

    return SynonymTable(std::move(from), *entities);
  }

  auto operator()(Synonym src, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByTypes(relationship_type_, left_, right_);

    return SynonymTable({std::move(src)}, ExtractEntities(*relationships, true, false));
  }

  auto operator()([[maybe_unused]] Underscore underscore, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByTypes(relationship_type_, left_, right_);
    return SynonymTable({std::move(dest)}, ExtractEntities(*relationships, false, true));
  }

  auto operator()(Synonym src, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByTypes(relationship_type_, left_, right_);

    return SynonymTable({std::move(src), std::move(dest)}, ExtractEntities(*relationships, true, true));
  }

  auto operator()([[maybe_unused]] Underscore underscore_1, [[maybe_unused]] Underscore underscore_2) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByTypes(relationship_type_, left_, right_);

    return !(relationships->empty());
  }

 private:
  QueryFacade &pkb_;
  const RelationshipType &relationship_type_;
  const EntityType &left_;
  const EntityType &right_;

};

auto GetEntityType(Synonym &syn, std::vector<Declaration> &declarations) -> const EntityType & {
  auto decl = Declaration::findDeclarationWithSynonym(declarations, syn);
  if (!decl) {
    throw std::invalid_argument("Synonym in clause not in query declaration");
  }
  return DesignEntityToEntityType(decl->getDesignEntity());
}

auto SuchThatEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  const auto &relationship_type = RelationshipToRelationshipType(clause_.getRelationship());
  auto left = clause_.getArg1();
  auto right = clause_.getArg2();

  const auto
      &left_entity_type = std::holds_alternative<Synonym>(left) ? GetEntityType(std::get<Synonym>(left), declarations_)
                                                                : DesignEntityToEntityType(AllowedDesignEntityOnLeft.at(
          clause_.getRelationship()));
  const auto &right_entity_type =
      std::holds_alternative<Synonym>(right) ? GetEntityType(std::get<Synonym>(right), declarations_)
                                             : DesignEntityToEntityType(AllowedDesignEntityOnRight.at(clause_.getRelationship()));

  auto clause_visitor = ClauseVisitor(pkb, relationship_type, left_entity_type, right_entity_type);
  return std::visit(clause_visitor, left, right);
}

}  // namespace qps
