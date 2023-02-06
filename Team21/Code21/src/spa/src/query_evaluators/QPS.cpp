#include "QPS.h"

#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"
#include "query_evaluator.h"

namespace qps {
void QPS::evaluate(std::string &query_str, std::list<std::string> &results, QueryFacade &pkb) {
  QueryTokenizer tokenizer(query_str);
  QueryParser parser(tokenizer.tokenize());
  QueryEvaluator evaluator(parser.parse());
  for (const auto &str : evaluator.evaluateQuery(pkb)) {
    results.push_back(str);
  }
}
} // qps
