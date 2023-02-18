#include "assign_evaluator.h"
#include "query/query_exceptions.h"

#include <stack>
#include <cctype>

namespace qps {
    std::vector<Entity> AssignEvaluator::CallPkb(QueryFacade& pkb) {
        std::vector<Entity> res;
        Ref ref = clause_.getArg1();
        std::vector<AssignStatement> pkb_res;
        if (std::holds_alternative<QuotedIdentifier>(&ref)) {
            auto pkb_mid = pkb.getModifiesRelationshipsByLeftAndRightEntityTypes(EntityType::ASSIGN_STATEMENT, EntityType::VARIABLE);
            for (auto stmt : *pkb_mid) {
                if (stmt->getRightHandEntity() == std::get_if<QuotedIdentifier>(ref)) {
                    pkb_res->push_back(stmt->getLeftHandEntity());
                }
            }
        }
        else {
            auto pkb_mid = pkb.getAllAssignStatements();
            for (auto stmt : *pkb_mid) {
                pkb_res->push_back(*stmt);
            }
        }
        ExpressionSpec expr = clause_.getArg2();
        if (std::holds_alternative<QuotedIdentifier>(&expr)) {
            res.assign(pkb_res->begin(), pkb_res->end());
            return res;
        }
        else {
            Expression express = std::get_if<Expression>(expr);
            std::string infix = express.getExpression();
            bool is_partial = express.isExpressionPartial();
            std::string postfix = makePostfix(infix);
            return checkExpressionContained(pkb_res, postfix, is_partial);
        }
    }

    int AssignEvaluator::postfixHelper(char a) {
        switch (a)
        {
        case '*':
        case '/':
            return 1;
        case '+':
        case '-':
            return 0;
        default:
            return -1;
        }
    }

    std::string AssignEvaluator::makePostfix(std::string str) {
        std::stack<std::string> stck;
        std::string postfixed;

        int i = 0;
        int last = str.length() - 1;
        while (i < str.length()) {
            if (isalpha(s[i])) {
                postfixed += ("\"" + s[i]);
                i++;
                while (i < str.length() && isalnum(s[i])) {
                    postfixed += s[i];
                    i++;
                }
                postfixed += "\"";
            }
            else if (isdigit(s[i])) {
                postfixed += ("\"" + s[i]);
                i++;
                if (s[i - 1] == '0') {
                    if (i < str.length() && isdigit(s[i])) {
                        throw QueryException(ErrorType::Syntactic, "Syntactic error. Expression spec contains leading zero");
                    }
                }
                while (i < str.length() && isdigit(s[i])) {
                    postfixed += s[i];
                    i++;
                }
                postfixed += "\"";
            }
            else if (s[i] == '(' && i < last) {
                if (!isalnum(s[i + 1]) && s[i + 1] != '(') {
                    throw QueryException(ErrorType::Syntactic, "Syntactic error. ( is followed by a non alphanumeric char");
                }
                stck.push(s[i]);
                i++;
            }
            else if (s[i] == ')') {
                while (stck.top() != '(') {
                    postfixed += stck.top();
                    stck.pop();
                }
                stck.pop();
                i++;
            }
            else if (postfixHelper(s[i]) >= 0 && i < last) {
                if (s[i + 1] == ')') {
                    throw QueryException(ErrorType::Syntactic, "Syntactic error. operator is followed by )");
                }
                while ((postfixHelper(s[i]) <= postfixHelper(stck.top())) && !stck.empty()) {
                    postfixed += stck.top();
                    stck.pop();
                }
                stck.push(s[i]);
                i++;
            }
            else {
                throw QueryException(ErrorType:Syntactic, 
                    "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
            }
        }
        while (!stck.empty()) {
            postfixed += stck.top();
            stck.pop();
        }
        return postfixed;
    }

    std::vector<Entity> AssignEvaluator::checkExpressionContained(std::vector<AssignStatement> pkb_res, std::string postfix, bool is_partial) {
        // Comment out first as uses methods from PKB version 2
        /*std::vector<Entity> results;
        if (is_partial) {
            for (auto stmt : pkb_res) {
                size_t len = (stmt -> getPostFixExpression()).find(postfix);
                if (len > 0) {
                    results.push_back(stmt);
                }
            }
        }
        else {
            for (auto stmt : pkb_res) {
                if (postfix == (stmt -> getPostFixExpression())) {
                    results.push_back(stmt);
                }
            }
        }
        return results;*/
    }
} // qps
