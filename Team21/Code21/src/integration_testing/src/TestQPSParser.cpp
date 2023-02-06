#include <utility>

#include "catch.hpp"
#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"

TEST_CASE("End to end") {
  qps::QueryTokenizer tokeniser("variable v; Select v");
  auto tokens = tokeniser.tokenize();
  qps::QueryParser parser(tokens);
  auto query = parser.parse();
  REQUIRE(query.getSelectClause().getDesignEntity() == qps::DesignEntity::VARIABLE);
  REQUIRE(query.getSelectClause().getSynonym().getSynonym() == "v");
}
