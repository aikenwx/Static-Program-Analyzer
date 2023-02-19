#include "assign_evaluator.h"
#include "query/query_exceptions.h"

#include <stack>
#include <cctype>

namespace qps {
std::vector<Relationship *> AssignEvaluator::CallPkb(QueryFacade &pkb) {
  std::vector<ModifiesRelationship *> modifies_relationships;
  Ref ref1 = clause_.getArg1();

  auto all_assignment_variable_pairs =
      pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType::ASSIGN_STATEMENT, EntityType::VARIABLE);
  for (const auto &row : *all_assignment_variable_pairs) {
    if (std::holds_alternative<QuotedIdentifier>(ref1)
        && *row->getRightHandEntity()->getEntityValue() != std::get<QuotedIdentifier>(ref1).getQuotedId()) {
      continue;
    } else {
      modifies_relationships.push_back(row);
    }
  }

  ExpressionSpec expr = clause_.getArg2();
  if (std::holds_alternative<Expression>(expr)) {
    Expression express = std::get<Expression>(expr);
    std::string infix = express.getExpression();
    bool is_partial = express.isExpressionPartial();
    std::string postfix = makePostfix(infix);
    modifies_relationships = checkExpressionContained(modifies_relationships, postfix, is_partial);
  }

  std::vector<Relationship *> res;
  res.assign(modifies_relationships.begin(), modifies_relationships.end());
  return res;
}

int AssignEvaluator::postfixHelper(char a) {
  switch (a) {
    case '*':
    case '/':return 1;
    case '+':
    case '-':return 0;
    default:return -1;
  }
}

std::string AssignEvaluator::makePostfix(std::string s) {
  std::stack<char> stck;
  std::string postfixed;

  int i = 0;
  int last = s.length() - 1;
  while (i < s.length()) {
    if (isalpha(s[i])) {
      postfixed += "\"";
      postfixed += s[i];
      i++;
      while (i < s.length() && isalnum(s[i])) {
        postfixed += s[i];
        i++;
      }
      postfixed += "\"";
    } else if (isdigit(s[i])) {
      postfixed += "\"";
      postfixed += s[i];
      i++;
      if (s[i - 1] == '0') {
        if (i < s.length() && isdigit(s[i])) {
          throw QueryException(ErrorType::Syntactic, "Syntactic error. Expression spec contains leading zero");
        }
      }
      while (i < s.length() && isdigit(s[i])) {
        postfixed += s[i];
        i++;
      }
      postfixed += "\"";
    } else if (s[i] == '(' && i < last) {
      if (!isalnum(s[i + 1]) && s[i + 1] != '(') {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. ( is followed by a non alphanumeric char");
      }
      stck.push(s[i]);
      i++;
    } else if (s[i] == ')') {
      while (stck.top() != '(') {
        postfixed += stck.top();
        stck.pop();
      }
      stck.pop();
      i++;
    } else if (postfixHelper(s[i]) >= 0 && i < last) {
      if (s[i + 1] == ')') {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. operator is followed by )");
      }
      while ((postfixHelper(s[i]) <= postfixHelper(stck.top())) && !stck.empty()) {
        postfixed += stck.top();
        stck.pop();
      }
      stck.push(s[i]);
      i++;
    } else {
      throw QueryException(ErrorType::Syntactic,
                           "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
    }
  }
  while (!stck.empty()) {
    postfixed += stck.top();
    stck.pop();
  }
  return postfixed;
}

std::vector<ModifiesRelationship *> AssignEvaluator::checkExpressionContained(std::vector<ModifiesRelationship *> assignment_modifies,
                                                                              std::string postfix,
                                                                              bool is_partial) {
  std::vector<ModifiesRelationship *> filtered_relationships;
  for (auto relationship : assignment_modifies) {
    auto assignment_stmt = dynamic_cast<AssignStatement *>(relationship->getLeftHandEntity());
    auto post_fix_espression = *assignment_stmt->getPostFixExpression();
    bool expression_contained =
        (is_partial && post_fix_espression.find(postfix) != std::string::npos) || (postfix == post_fix_espression);
    if (expression_contained) {
      filtered_relationships.push_back(relationship);
    }
  }
  return filtered_relationships;

}
} // qps
