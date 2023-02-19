#include <utility>

#include "catch.hpp"

#include "query_evaluators/join/join.h"

bool CheckEqual(const qps::SynonymTable &table1, const qps::SynonymTable table2) {
  if (table1.GetSynonyms() != table2.GetSynonyms()) {
    return false;
  }
  std::vector<qps::SynonymTable::Row> table1_res = table1.GetResults();
  std::vector<qps::SynonymTable::Row> table2_res = table2.GetResults();
  std::sort(table1_res.begin(), table1_res.end());
  std::sort(table2_res.begin(), table2_res.end());
  return table1_res == table2_res;
}

TEST_CASE("Simple Join with tables made of the same 1 synonym") {
  qps::Synonym a{"a"};

  qps::SynonymTable table1({a}, {{"1"}, {"2"}, {"10"}});
  qps::SynonymTable table2({a}, {{"2"}, {"5"}, {"10"}, {"11"}});

  qps::SynonymTable expected({a}, {{"2"}, {"10"}});
  qps::SynonymTable res = qps::HashJoin{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));
}

TEST_CASE("Cross Join with 1 synonym table with no common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};

  qps::SynonymTable table1({a}, {{"1"}, {"2"}});
  qps::SynonymTable table2({b}, {{"4"}, {"5"}});

  qps::SynonymTable expected({a, b}, {{"1", "4"}, {"1", "5"}, {"2", "4"}, {"2", "5"}});
  qps::SynonymTable res = qps::HashJoin{}(table1, table2);
  REQUIRE(CheckEqual(res, expected));
}

TEST_CASE("Join with 2 synonym tables with 1 common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};
  qps::Synonym c{"c"};

  qps::SynonymTable table1{{a, b}, {{"1", "z"}, {"2", "z"}, {"5", "y"}, {"11", "a"}}};
  qps::SynonymTable table2{{b, c}, {{"z", "proc"}, {"a", "cloc"}}};

  qps::SynonymTable expected({a, b, c}, {{"1", "z", "proc"}, {"2", "z", "proc"}, {"11", "a", "cloc"}});
  qps::SynonymTable res = qps::HashJoin{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));

  qps::SynonymTable table3{{c, b}, {{"proc", "z"}, {"cloc", "a"}}};
  qps::SynonymTable res2 = qps::HashJoin{}(table1, table3);

  REQUIRE(CheckEqual(res2, expected));
}

TEST_CASE("Join with 2 synonym tables with 2 common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};
  qps::Synonym c{"c"};

  qps::SynonymTable table1{{a, b}, {{"1", "z"}, {"2", "z"}, {"5", "y"}, {"11", "a"}}};
  qps::SynonymTable table2{{a, b}, {{"1", "z"}, {"1", "y"}, {"2", "a"}, {"11", "a"}, {"11", "b"}}};

  qps::SynonymTable expected({a, b}, {{"1", "z"}, {"11", "a"}});
  qps::SynonymTable res = qps::HashJoin{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));
}

