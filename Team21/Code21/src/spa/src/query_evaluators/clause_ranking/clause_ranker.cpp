#include "clause_ranker.h"

#include "query/ref.h"
#include "query/with_ref.h"

namespace qps {

int DesignEntityScore(DesignEntity entity) {
  switch (entity) {
    case DesignEntity::PROCEDURE:return 2;
    case DesignEntity::WHILE:return 5;
    case DesignEntity::IF:return 5;
    case DesignEntity::READ:return 20;
    case DesignEntity::PRINT:return 20;
    case DesignEntity::CALL:return 20;
    case DesignEntity::ASSIGN:return 40;
    case DesignEntity::STMT: return 100;
    case DesignEntity::VARIABLE: return 200;
    case DesignEntity::CONSTANT: return 200;
  }
}

int RelationshipScore(Relationship relationship) {
  switch (relationship) {
    case Relationship::Next:return 1;
    case Relationship::Parent:return 2;
    case Relationship::Follows:return 3;
    case Relationship::Calls:return 4;
    case Relationship::Uses:return 5;
    case Relationship::UsesS:return 6;
    case Relationship::UsesP:return 7;
    case Relationship::Modifies:return 8;
    case Relationship::ModifiesS:return 9;
    case Relationship::ModifiesP:return 10;
    case Relationship::ParentT:return 11;
    case Relationship::FollowsT:return 12;
    case Relationship::CallsT:return 13;
    case Relationship::NextT:return 14;
    case Relationship::Affects:return 15;
    case Relationship::AffectsT:return 16;
  }
}

auto SynonymScore(const Synonym &syn, const std::vector<Declaration> &decl_lst) {
  auto design_entity = Declaration::findDeclarationWithSynonym(decl_lst, syn).value().getDesignEntity();
  return DesignEntityScore(design_entity);
}

struct RefScorer {
  template<typename Literal>
  auto operator()(const Literal &syn) -> int {
    return 1;
  }

  auto operator()(const Synonym &syn) -> int {
    return SynonymScore(syn, decl_lst);
  }

  auto operator()(const Underscore &underscore) -> int {
    return 100;
  }

  const std::vector<Declaration> &decl_lst;
};

struct WithRefScorer {
  template<typename Literal>
  auto operator()(const Literal &syn) -> int {
    return 1;
  }
  auto operator()(const AttrRef &ref) -> int {
    return SynonymScore(ref.synonym, decl_lst);
  }

  const std::vector<Declaration> &decl_lst;
};

auto RefScore(const SuchThatClause &clause, const std::vector<Declaration> &decl_lst) -> int {
  auto scorer = RefScorer{decl_lst};
  return std::visit(scorer, clause.getArg1()) * std::visit(scorer, clause.getArg2());
}

auto RefScore(const WithClause &clause, const std::vector<Declaration> &decl_lst) -> int {
  auto scorer = WithRefScorer{decl_lst};
  return std::visit(scorer, clause.getRef1().ref) * std::visit(scorer, clause.getRef2().ref);
}

void ClauseRanker::SortSuchThatClause(std::vector<SuchThatClause> &clauses) {
  std::sort(std::begin(clauses), std::end(clauses),
            [&](const SuchThatClause &clause1, const SuchThatClause &clause2) {
              auto clause1_score = RefScore(clause1, decl_lst_);
              auto clause2_score = RefScore(clause2, decl_lst_);
              if (clause1_score == clause2_score) {
                return RelationshipScore(clause1.getRelationship()) < RelationshipScore(clause2.getRelationship());
              }
              return clause1_score < clause2_score;
            });
}

void ClauseRanker::SortWithClause(std::vector<WithClause> &clauses) {
  std::sort(std::begin(clauses), std::end(clauses),
            [&](const WithClause &clause1, const WithClause &clause2) {
              auto clause1_score = RefScore(clause1, decl_lst_);
              auto clause2_score = RefScore(clause2, decl_lst_);
              return clause1_score <= clause2_score;
            });
}

void ClauseRanker::SortPatternClause(std::vector<PatternClause> &clauses) {
  std::sort(std::begin(clauses), std::end(clauses),
            [&](const PatternClause &clause1, const PatternClause &clause2) {
              auto clause1_syn_score = SynonymScore(clause1.getStmtSynonym(), decl_lst_);
              auto clause2_syn_score = SynonymScore(clause2.getStmtSynonym(), decl_lst_);
              if (clause1_syn_score == clause2_syn_score) {
                auto scorer = RefScorer{decl_lst_};
                return std::visit(scorer, clause1.getArg1()) < std::visit(scorer, clause2.getArg1());
              }
              return clause1_syn_score < clause2_syn_score;
            });
}

void ClauseRanker::SortQuery(Query &query) {
  SortWithClause(query.getWithClause());
  SortPatternClause(query.getPatternClause());
  SortSuchThatClause(query.getSuchThatClause());
}

} // qps
