#include "relationship_evaluator.h"

#include <utility>
#include "query/ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/pkb_helpers.h"

namespace qps {

class ClauseVisitor {
 public:
  ClauseVisitor(QueryFacade &pkb, std::vector<Declaration> &declarations, RelationshipType relationship,
                EntityType left, EntityType right)
      : pkb_(pkb), declarations_(declarations), relationship_type_(relationship),
        left_(left), right_(right) {}

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
    auto *relationships =
        pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_, left_, from, right_);
    return relationships != nullptr;
  }

  auto operator()(QuotedIdentifier from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *relationships = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                                     left_,
                                                                                     from.getQuotedId(),
                                                                                     right_);
    return relationships != nullptr;
  }

  auto operator()([[maybe_unused]] Underscore underscore, StatementNumber dest) -> ClauseResult {
    auto *relationships =
        pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_, left_, right_, dest);
    return relationships != nullptr;
  }

  auto operator()([[maybe_unused]] Underscore underscore, QuotedIdentifier dest) -> ClauseResult {
    auto *relationships = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                                     left_,
                                                                                     right_,
                                                                                     dest.getQuotedId());
    return relationships != nullptr;
  }

  auto operator()(StatementNumber src, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                                   left_,
                                                                                   src,

                                                                                   right_);
    return ConstructSynonymTable(*relationships, {dest}, ExtractRightEntity);

  }

  auto operator()(Synonym from, StatementNumber dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                                   left_,
                                                                                   right_,
                                                                                   dest);
    return ConstructSynonymTable(*relationships, {from}, ExtractLeftEntity);
  }

  auto operator()(QuotedIdentifier src, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                                   left_,
                                                                                   src.getQuotedId(),
                                                                                   right_);
    return ConstructSynonymTable(*relationships, {dest}, ExtractRightEntity);

  }

  auto operator()(Synonym from, QuotedIdentifier dest) -> ClauseResult {
    auto *
        relationships = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                                   left_,
                                                                                   right_,
                                                                                   dest.getQuotedId());
    return ConstructSynonymTable(*relationships, {from}, ExtractLeftEntity);
  }

  auto operator()(Synonym src, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *
        relationships = pkb_.getFollowsRelationshipsByLeftAndRightEntityTypes(left_, right_);
    return ConstructSynonymTable(*relationships, {src}, ExtractLeftEntity);
  }

  auto operator()([[maybe_unused]] Underscore underscore, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getFollowsRelationshipsByLeftAndRightEntityTypes(left_, right_);
    return ConstructSynonymTable(*relationships, {dest}, ExtractRightEntity);
  }

  auto operator()(Synonym src, Synonym dest) -> ClauseResult {
    auto *
        relationships = pkb_.getFollowsRelationshipsByLeftAndRightEntityTypes(left_, right_);
    return ConstructSynonymTable(*relationships, {src, dest}, ExtractBothEntities);
  }

  auto operator()([[maybe_unused]] Underscore underscore_1, [[maybe_unused]] Underscore underscore_2) -> ClauseResult {
    auto *
        relationships = pkb_.getFollowsRelationshipsByLeftAndRightEntityTypes(left_, right_);
    return relationships != nullptr;
  }

 private:
  QueryFacade &pkb_;
  std::vector<Declaration> &declarations_;
  RelationshipType relationship_type_;
  EntityType left_;
  EntityType right_;

  template<class Extractor>
  auto ConstructSynonymTable(const std::vector<::Relationship *> &relationships,
                             std::vector<Synonym> synonyms,
                             Extractor extractor) {
    SynonymTable table(std::move(synonyms));
    for (const auto &relationship : relationships) {
      table.AddRow(extractor(relationship));
    }
    return table;
  }
};

auto RelationshipEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {

//  auto left_entity_types = RelationshipToRelationshipType(clause_.getRelationship());
//  auto right_entity_types = RelationshipToRelationshipType(clause_.getRelationship());


}
}  // namespace qps
