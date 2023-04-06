#pragma once

#include "query/expression.h"
#include "query/ref.h"
#include "query/synonym.h"
#include "query/underscore.h"

#include <variant>

namespace qps {

using ExpressionSpec = std::variant<Underscore, Expression>;

class PatternClause {
 private:
  Synonym stmtSynonym;
  Ref arg1;
  ExpressionSpec arg2;
  bool gotExpression;
  bool isPartial;

 public:
  PatternClause(Synonym synonym_, Ref arg1_, ExpressionSpec arg2);
  [[maybe_unused]] [[nodiscard]] auto clauseGotExpression() const -> bool;
  [[maybe_unused]] [[nodiscard]] auto isExpressionPartial() const -> bool;
  auto getArg1() const -> const Ref &;
  auto getArg2() const -> const ExpressionSpec &;
  auto getStmtSynonym() const -> const Synonym &;

  auto operator==(const PatternClause &clause) const -> bool {
    return stmtSynonym == clause.stmtSynonym && arg1 == clause.arg1 &&
        arg2 == clause.arg2 && gotExpression == clause.gotExpression &&
        isPartial == clause.isPartial;
  }

  friend auto operator<<(std::ostream &ostream, PatternClause &clause)
  -> std::ostream & {
    ostream << "Pattern " << clause.stmtSynonym.getSynonym();
    return ostream;
  }
};

} // namespace qps
