#include "such_that_evaluator.h"

#include "query/ref.h"
#include "query/query_exceptions.h"
#include "query_evaluators/pkb_qps_type_mapping.h"

namespace qps {

int GetLiteral(const StatementNumber &num) {
  return num;
}

auto GetLiteral(const QuotedIdentifier &id) -> std::string {
  return id.getQuotedId();
}

class ClauseVisitor {
 public:
  ClauseVisitor(QueryFacade &pkb, const RelationshipType &relationship, const EntityType &left, const EntityType &right)
      : pkb_(pkb), relationship_type_(relationship), left_(left), right_(right) {}

  template<typename Literal1, typename Literal2>
  auto operator()(Literal1 from, Literal2 dest) -> ClauseResult {
    auto *relationship = pkb_.getRelationship(relationship_type_, left_, GetLiteral(from), right_, GetLiteral(dest));
    return relationship != nullptr;
  }

  template<typename Literal>
  auto operator()(Literal from, [[maybe_unused]] Underscore underscore) -> ClauseResult {
    auto *entities =
        pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_, left_, GetLiteral(from), right_);
    return !(entities->empty());
  }

  template<typename Literal>
  auto operator()([[maybe_unused]] Underscore underscore, Literal dest) -> ClauseResult {
    auto *entities =
        pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_, left_, right_, GetLiteral(dest));
    return !(entities->empty());
  }

  template<typename Literal>
  auto operator()(Literal src, Synonym dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityLiteralAndRightEntityType(relationship_type_,
                                                                              left_,
                                                                              GetLiteral(src),
                                                                              right_);
    return SynonymTable(std::move(dest), *entities);
  }

  template<typename Literal>
  auto operator()(Synonym from, Literal dest) -> ClauseResult {
    auto *
        entities = pkb_.getRelationshipsByLeftEntityTypeAndRightEntityLiteral(relationship_type_,
                                                                              left_,
                                                                              right_,
                                                                              GetLiteral(dest));
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
    bool require_equal = false;
    if (src == dest) {
      require_equal = true;
    }
    return SynonymTable({std::move(src), std::move(dest)}, ExtractEntities(*relationships, true, true, require_equal));
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

  auto ExtractEntities(const std::vector<::Relationship *> &relationships,
                       bool left,
                       bool right,
                       bool require_equal = false) -> std::vector<std::vector<Entity *>> {
    std::vector<std::vector<Entity *>> rows;
    rows.reserve(relationships.size());
    for (const auto &relationship : relationships) {
      if (require_equal && relationship->getLeftHandEntity() != relationship->getRightHandEntity()) {
        continue;
      }

      std::vector<Entity *> row;
      if (left) {
        row.push_back(relationship->getLeftHandEntity());
      }

      if (right) {
        row.push_back(relationship->getRightHandEntity());
      }

      rows.push_back(std::move(row));
    }
    return rows;
  }
};

auto GetEntityType(const Synonym &syn, const std::vector<Declaration> &declarations) -> const EntityType & {
  auto decl = Declaration::findDeclarationWithSynonym(declarations, syn);
  if (!decl) {
    throw std::invalid_argument("Synonym in clause not in query declaration");
  }
  return DesignEntityToEntityType(decl->getDesignEntity());
}

auto SuchThatEvaluator::Evaluate(QueryFacade &pkb) -> ClauseResult {
  const auto &relationship_type = RelationshipToRelationshipType(clause_.getRelationship());
  const auto &left = clause_.getArg1();
  const auto &right = clause_.getArg2();

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
