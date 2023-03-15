#include <utility>

#include "catch.hpp"

#include "query_evaluators/tables/row_table.h"
#include "query_evaluators/join/join.h"
#include "query/synonym.h"

using Table = qps::RowTable<qps::Synonym, std::string>;
using Join = qps::HashJoin<Table>;

template<typename Table>
bool CheckEqual(const Table &table1, const Table table2) {
  if (table1.HeaderSize() != table2.HeaderSize() || table1.ResultSize() != table2.ResultSize()) {
    return false;
  }
  auto header1 = table1.GetHeader();
  std::sort(header1.begin(), header1.end());
  auto header2 = table2.GetHeader();
  std::sort(header2.begin(), header2.end());
  if (header1 != header2) {
    return false;
  }

  auto rows1 = table1.GetRows();
  std::sort(rows1.begin(), rows1.end());
  auto rows2 = table2.GetRows();
  std::sort(rows2.begin(), rows2.end());

  for (size_t row = 0; row < rows1.size(); ++row) {
    auto row1 = rows1[row];
    auto row2 = rows1[row];
    std::sort(row1.begin(), row1.end());
    std::sort(row2.begin(), row2.end());
    if (row1 != row2) return false;
  }
  return true;
}

TEST_CASE("Simple Join with tables made of the same 1 synonym") {
  qps::Synonym a{"a"};

  Table table1({a}, {{"1"}, {"2"}, {"10"}});
  Table table2({a}, {{"2"}, {"5"}, {"10"}, {"11"}});

  Table expected({a}, {{"2"}, {"10"}});
  Table res = Join{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));
}

TEST_CASE("Cross Join with 1 synonym table with no common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};

  Table table1({a}, {{"1"}, {"2"}});
  Table table2({b}, {{"4"}, {"5"}});

  Table expected({a, b}, {{"1", "4"}, {"1", "5"}, {"2", "4"}, {"2", "5"}});
  Table res = Join{}(table1, table2);
  for (auto &row : res.GetRows()) {
    for (auto &k : row) {
      std::cout << k << " ";
    }
    std::cout << "\n";
  }
  REQUIRE(CheckEqual(res, expected));
}

TEST_CASE("Join with 2 synonym tables with 1 common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};
  qps::Synonym c{"c"};

  Table table1{{a, b}, {{"1", "z"}, {"2", "z"}, {"5", "y"}, {"11", "a"}}};
  Table table2{{b, c}, {{"z", "proc"}, {"a", "cloc"}}};

  Table expected({a, b, c}, {{"1", "z", "proc"}, {"2", "z", "proc"}, {"11", "a", "cloc"}});
  Table res = Join{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));

  Table table3{{c, b}, {{"proc", "z"}, {"cloc", "a"}}};
  Table res2 = Join{}(table1, table3);

  REQUIRE(CheckEqual(res2, expected));
}

TEST_CASE("Join with 2 synonym tables with 2 common synonym") {
  qps::Synonym a{"a"};
  qps::Synonym b{"b"};
  qps::Synonym c{"c"};

  Table table1{{a, b}, {{"1", "z"}, {"2", "z"}, {"5", "y"}, {"11", "a"}}};
  Table table2{{a, b}, {{"1", "z"}, {"1", "y"}, {"2", "a"}, {"11", "a"}, {"11", "b"}}};

  Table expected({a, b}, {{"1", "z"}, {"11", "a"}});
  Table res = Join{}(table1, table2);

  REQUIRE(CheckEqual(res, expected));
}

