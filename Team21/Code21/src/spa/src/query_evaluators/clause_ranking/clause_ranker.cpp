#include "clause_ranker.h"

#include "query/ref.h"
#include "query/with_ref.h"

#include "algorithm"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace qps {
// Rough Heuristic to rank relative number of each design entity
// This will depend on the source so this is just an estimation based on some general principles
// Generally Assign Statements exceed other type of statements (while and if will tend to be the least)
// (here we estimate they are 4 times more than read or print statements for example)
// Variable & Constants don't have any limit so they can potentially be much more than number of statements
// (estimated to be slightly more than 4 times more than stmts).
auto DesignEntityScore(DesignEntity entity) -> int {
  switch (entity) {
    case DesignEntity::PROCEDURE:return 1;
    case DesignEntity::WHILE:return 25;
    case DesignEntity::IF:return 25;
    case DesignEntity::READ:return 50;
    case DesignEntity::PRINT:return 50;
    case DesignEntity::CALL:return 50;
    case DesignEntity::ASSIGN:return 200;
    case DesignEntity::STMT: return 400;
    case DesignEntity::VARIABLE: return 5000;
    case DesignEntity::CONSTANT: return 5000;
  }
}

// Rough Heuristic to rank each clause in terms of number of results it might return
// These numbers are not relative because we are never comparing clauses in aggregate
// (our grouping is one on the fly as we evaluate the clauses one by one)
auto RelationshipSizeScore(Relationship relationship) -> int {
  switch (relationship) {
    case Relationship::Next:return 1;
    case Relationship::Parent:return 2;
    case Relationship::Calls:return 3;
    case Relationship::Follows:return 4;
    case Relationship::Modifies:return 5;
    case Relationship::ModifiesS:return 6;
    case Relationship::ModifiesP:return 7;
    case Relationship::Uses:return 8;
    case Relationship::UsesS:return 9;
    case Relationship::UsesP:return 10;
    case Relationship::ParentT:return 11;
    case Relationship::CallsT:return 12;
    case Relationship::FollowsT:return 13;
    case Relationship::Affects:return 14;
    case Relationship::NextT:return 15;
    case Relationship::AffectsT:return 16;
  }
}

// Rough Heuristic to rank each clause in terms of time needed to calculate results
// Except for NextT, Affects, AffectsT everything is precomputed (so they are all given the same score)
auto RelationshipCalculationScore(Relationship relationship) -> int {
  switch (relationship) {
    case Relationship::Parent:
    case Relationship::Follows:
    case Relationship::Calls:
    case Relationship::Uses:
    case Relationship::UsesS:
    case Relationship::UsesP:
    case Relationship::Modifies:
    case Relationship::ModifiesS:
    case Relationship::ModifiesP:
    case Relationship::ParentT:
    case Relationship::FollowsT:
    case Relationship::CallsT:return 1;
    case Relationship::Next:return 2;
    case Relationship::NextT:return 3;
    case Relationship::Affects:return 4;
    case Relationship::AffectsT:return 5;
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
    // Underscore might actually represent an entity lesser than this or more than this
    // depending on the clause. STMT is just chosen as an average as a heuristic.
    return DesignEntityScore(DesignEntity::STMT);
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

const std::vector<Relationship> NotPrecomputed = {Relationship::Next, Relationship::Affects, Relationship::AffectsT};

// Compare time needed to calculate first. Push any relationship that takes time to calculate to the back
// If calculation time is the same, then compare, number of literals or synonyms in clause. Clause with more literals
// or more restrictive synonyms should be done first
// If the number of literals and synonyms is the same then compare rough size expected based on relationship
void ClauseRanker::SortSuchThatClause(std::vector<SuchThatClause> &clauses) {
  std::sort(std::begin(clauses), std::end(clauses),
            [&](const SuchThatClause &clause1, const SuchThatClause &clause2) {
              auto clause1_calc_score = RelationshipCalculationScore(clause1.getRelationship());
              auto clause2_calc_score = RelationshipCalculationScore(clause2.getRelationship());
              if (clause1_calc_score != clause2_calc_score) {
                return clause1_calc_score < clause2_calc_score;
              }
              auto clause1_score = RefScore(clause1, decl_lst_);
              auto clause2_score = RefScore(clause2, decl_lst_);
              if (clause1_score == clause2_score) {
                return RelationshipSizeScore(clause1.getRelationship())
                    < RelationshipSizeScore(clause2.getRelationship());
              }
              return clause1_score < clause2_score;
            });
}

// Compare solely based on number and type of synonym present in the with clause
void ClauseRanker::SortWithClause(std::vector<WithClause> &clauses) {
  std::sort(std::begin(clauses), std::end(clauses),
            [&](const WithClause &clause1, const WithClause &clause2) {
              auto clause1_score = RefScore(clause1, decl_lst_);
              auto clause2_score = RefScore(clause2, decl_lst_);
              return clause1_score <= clause2_score;
            });
}

// Compare solely based on pattern synonym first (num of if and while statements should generally be less than assign)
// In the case the pattern synonym is same then compare based on presence of synonyms in clause
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
}  // namespace qps

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
