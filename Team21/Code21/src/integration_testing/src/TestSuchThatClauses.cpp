#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"
#include "query/design_entity.h"

//procedure computeCentroid {
//  count = 0; 01
//  cenX = 0; 02
//  cenY = 0; 03
//  call readPoint; 04
//
//  while((x != 0) && (y != 0)) { 05
//    if (y != 0) then { 06
//      z = cenX + cenY; 07
//      call readPoint; 08
//      print z; 09
//    } else {
//       read count; 10
//    }
//    count = count + 1; 11
//    cenX = cenX + x; 12
//    cenY = cenY + y; 13
//    call readPoint; 14
//  }
//
//  if (count == 0) then { 15
//    while (flag == 0) { 16
//      flag = 2; 17
//    }
//    flag = 1; 18
//  } else {
//    cenX = cenX / count; 19
//    cenY = cenY / count; 20
//  }
//  normSq = cenX * cenX + cenY * cenY; 21
//}



qps_test::PopulatePKBHelper::Data PopulateEntities(qps_test::PopulatePKBHelper &pkb_helper) {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "flag", "x", "y", "z", "normSq"};
  data[qps::DesignEntity::CONSTANT] = {"0", "1", "2"};
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "7", "11", "12", "13", "17", "18", "19", "20", "21"};
  data[qps::DesignEntity::READ] = {"10"};
  data[qps::DesignEntity::CALL] = {"4", "8", "14"};
  data[qps::DesignEntity::IF] = {"6", "15"};
  data[qps::DesignEntity::WHILE] = {"5", "16"};
  data[qps::DesignEntity::PRINT] = {"9"};
  data[qps::DesignEntity::PROCEDURE] = {"computeCentroid", "readPoint"};

  data[qps::DesignEntity::STMT] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
                                   "13", "14", "15", "16", "17", "18", "19", "20", "21"};

  pkb_helper.PopulateEntities(data);

  return data;
}

TEST_CASE("QPS can work with different combinations of modifies") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementModifies({{1, "count"}, {2, "cenX"}, {3, "cenY"}, {5, "count"}, {5, "cenX"},
                                   {5, "cenY"}, {5, "z"}, {6, "z"}, {6, "count"}, {7, "z"}, {10, "count"},
                                   {11, "count"}, {12, "cenX"}, {13, "cenY"},
                                   {15, "flag"}, {15, "cenX"}, {15, "cenY"}, {16, "flag"}, {17, "flag"}, {18, "flag"},
                                   {19, "cenX"}, {20, "cenY"},
                                   {21, "normSq"}
                                  });
  pkb_helper.AddProcedureModifies({{"computeCentroid", "count"}, {"computeCentroid", "cenX"},
                                   {"computeCentroid", "cenY"}, {"computeCentroid", "z"},
                                   {"computeCentroid", "flag"},
                                   {"computeCentroid", "normSq"}});

  SECTION("Both Synonyms") {
    SECTION("All variables modified in a statement") {
      std::unordered_set<std::string>
          expected{"count", "cenX", "cenY", "flag", "normSq", "z"};
      REQUIRE(qps_test::RunQuery("variable v; stmt s; Select v such that Modifies(s, v)", *pkb_querier) == expected);
    }

    SECTION("All variables modified in a procedure") {
      std::unordered_set<std::string>
          expected{"count", "cenX", "cenY", "flag", "normSq", "z"};
      REQUIRE(
          qps_test::RunQuery("procedure p; variable v; Select v such that Modifies(p, v)", *pkb_querier) == expected);
    }

    SECTION("All statements modifiying a variable") {
      std::unordered_set<std::string>
          expected{"1", "2", "3", "5", "6", "7", "10", "11", "12", "13", "15", "16", "17", "18", "19", "20", "21"};
      REQUIRE(qps_test::RunQuery("variable v; stmt s; Select s such that Modifies(s, v)", *pkb_querier) == expected);
    }

    SECTION("All assigns modifying a variable") {
      REQUIRE(qps_test::RunQuery("variable v; assign a; Select a such that Modifies(a, v)", *pkb_querier)
                  == data[qps::DesignEntity::ASSIGN]);
    }

    SECTION("All whiles modifying a variable") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("variable v; while w; Select w such that Modifies(w, v)", *pkb_querier) == expected);
    }

    SECTION("All ifs modifying a variable") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("variable v; if f; Select f such that Modifies(f, v)", *pkb_querier) == expected);
    }

    SECTION("All procedures modifying a variable") {
      std::unordered_set<std::string> expected{"computeCentroid"};
      REQUIRE(
          qps_test::RunQuery("variable v; procedure p; Select p such that Modifies(p, v)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("All procedures modifying a variable") {
      std::unordered_set<std::string> expected{"computeCentroid"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Modifies(p, _)", *pkb_querier) == expected);
    }

    SECTION("All statements modifying a variable") {
      std::unordered_set<std::string>
          expected{"1", "2", "3", "5", "6", "7", "10", "11", "12", "13", "15", "16", "17", "18", "19", "20", "21"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Modifies(s, _)", *pkb_querier) == expected);
    }

    SECTION("All assignments modifying a variable") {
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Modifies(a, _)", *pkb_querier)
                  == data[qps::DesignEntity::ASSIGN]);
    }

    SECTION("All reads modifiying a variable") {
      std::unordered_set<std::string> expected{"10"};
      REQUIRE(qps_test::RunQuery("read r; Select r such that Modifies(r, _)", *pkb_querier) == expected);
    }

    SECTION("All whiles using a variable") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Modifies(w, _)", *pkb_querier) == expected);
    }

    SECTION("All ifs using a variable") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Modifies(f, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    SECTION("All statements modifying a specific variable") {
      std::unordered_set<std::string> expected{"2", "5", "12", "15", "19"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Modifies(s, \"cenX\")", *pkb_querier) == expected);
    }

    SECTION("All ifs modifying a specific variable") {
      std::unordered_set<std::string> expected{"6"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Modifies(f, \"z\")", *pkb_querier) == expected);
    }

    SECTION("All whiles modifying a specific variable") {
      std::unordered_set<std::string> expected{"5"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Modifies(w, \"count\")", *pkb_querier) == expected);
    }

    SECTION("All stmts modifying a specific variable") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Modifies(s, \"x\")", *pkb_querier) == empty);
    }
  }

  SECTION("Synonym on right, literal on left") {
    SECTION("All variables modified by a specific procedure") {
      std::unordered_set<std::string> expected{"count", "cenX", "cenY", "flag", "normSq", "z"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Modifies(\"computeCentroid\", v)", *pkb_querier)
              == expected);
    }

    SECTION("All variables modified by a specific if") {
      std::unordered_set<std::string> expected{"count", "z"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Modifies(6, v)", *pkb_querier) == expected);
    }

    SECTION("All variables modified by a specific while") {
      std::unordered_set<std::string> expected{"count", "z", "cenX", "cenY"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Modifies(5, v)", *pkb_querier) == expected);
    }
  }

  SECTION("Evaluates to true") {
    SECTION("Read statement modifies a variable") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Modifies(10, \"count\")", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("While statement modifies a variable") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Modifies(5, \"cenX\")", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("If statement modifies a variable") {
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Modifies(6, \"z\")", *pkb_querier)
                  == data[qps::DesignEntity::STMT]);
    }

  }

  SECTION("Evaluates to false") {
    std::unordered_set<std::string> empty{};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Modifies(s, \"x\")", *pkb_querier)
                == empty);
    REQUIRE(qps_test::RunQuery("if f; Select f such that Modifies(15, \"z\")", *pkb_querier)
                == empty);
  }
}

TEST_CASE("QPS can work with different combinations of uses") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddStatementUses({{7, "cenX"}, {7, "cenY"}, {9, "z"}, {11, "count"}, {12, "cenX"}, {12, "x"},
                               {13, "cenY"}, {13, "y"}, {6, "cenX"}, {6, "cenY"}, {6, "z"},
                               {5, "count"}, {5, "cenX"}, {5, "x"}, {5, "cenY"}, {5, "y"}, {5, "z"},
                               {19, "cenX"}, {19, "count"}, {20, "count"}, {20, "cenY"},
                               {15, "cenX"}, {15, "count"}, {15, "cenY"},
                               {21, "cenX"}, {21, "cenY"}
                              });

  pkb_helper.AddProcedureUses({{"computeCentroid", "cenX"}, {"computeCentroid", "cenY"}, {"computeCentroid", "count"},
                               {"computeCentroid", "x"}, {"computeCentroid", "y"},
                               {"computeCentroid", "z"}
                              });

  SECTION("Both Synonyms") {
    SECTION("All variables used in a statement") {
      std::unordered_set<std::string>
          expected{"count", "cenX", "cenY", "x", "y", "z"};
      REQUIRE(qps_test::RunQuery("variable v; stmt s; Select v such that Uses(s, v)", *pkb_querier) == expected);
    }

    SECTION("All variables used in a procedure") {
      std::unordered_set<std::string>
          expected{"count", "cenX", "cenY", "x", "y", "z"};
      REQUIRE(qps_test::RunQuery("procedure p; variable v; Select v such that Uses(p, v)", *pkb_querier) == expected);
    }

    SECTION("All statements using a variable") {
      std::unordered_set<std::string> expected{"7", "9", "11", "12", "13", "6", "5", "19", "20", "15", "21"};
      REQUIRE(qps_test::RunQuery("variable v; stmt s; Select s such that Uses(s, v)", *pkb_querier) == expected);
    }

    SECTION("All assigns using a variable") {
      std::unordered_set<std::string> expected{"7", "11", "12", "13", "19", "20", "21"};
      REQUIRE(qps_test::RunQuery("variable v; assign a; Select a such that Uses(a, v)", *pkb_querier) == expected);
    }

    SECTION("All whiles using a variable") {
      std::unordered_set<std::string> expected{"5"};
      REQUIRE(qps_test::RunQuery("variable v; while w; Select w such that Uses(w, v)", *pkb_querier) == expected);
    }

    SECTION("All ifs using a variable") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("variable v; if f; Select f such that Uses(f, v)", *pkb_querier) == expected);
    }

    SECTION("All procedures using a variable") {
      std::unordered_set<std::string> expected{"computeCentroid"};
      REQUIRE(qps_test::RunQuery("variable v; procedure p; Select p such that Uses(p, v)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("All procedures using a variable") {
      std::unordered_set<std::string> expected{"computeCentroid"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Uses(p, _)", *pkb_querier) == expected);
    }

    SECTION("All statements using a variable") {
      std::unordered_set<std::string> expected{"7", "9", "11", "12", "13", "6", "5", "19", "20", "15", "21"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Uses(s, _)", *pkb_querier) == expected);
    }

    SECTION("All assignments using a variable") {
      std::unordered_set<std::string> expected{"7", "11", "12", "13", "19", "20", "21"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Uses(a, _)", *pkb_querier) == expected);
    }

    SECTION("All prints using a variable") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Uses(pn, _)", *pkb_querier) == expected);
    }

    SECTION("All whiles using a variable") {
      std::unordered_set<std::string> expected{"5"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Uses(w, _)", *pkb_querier) == expected);
    }

    SECTION("All ifs using a variable") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Uses(f, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    SECTION("All statements using a specific variable") {
      std::unordered_set<std::string> expected{"5", "6", "7", "12", "19", "21", "15"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Uses(s, \"cenX\")", *pkb_querier) == expected);
    }

    SECTION("All ifs using a specific variable") {
      std::unordered_set<std::string> expected{"6"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Uses(f, \"z\")", *pkb_querier) == expected);
    }

    SECTION("All whiles using a specific variable") {
      std::unordered_set<std::string> expected{"5"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Uses(w, \"count\")", *pkb_querier) == expected);
    }

    SECTION("All stmts using a specific variable") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Uses(s, \"normSq\")", *pkb_querier) == empty);
    }
  }

  SECTION("Synonym on right, literal on left") {
    SECTION("All variables used by a specific procedure") {
      std::unordered_set<std::string> expected{"count", "cenX", "cenY", "x", "y", "z"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Uses(\"computeCentroid\", v)", *pkb_querier) == expected);
    }

    SECTION("All variables used by a specific if") {
      std::unordered_set<std::string> expected{"cenX", "cenY", "z"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Uses(6, v)", *pkb_querier) == expected);
    }

    SECTION("All variables used by a specific while") {
      std::unordered_set<std::string> expected{"count", "cenX", "cenY", "x", "y", "z"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Uses(5, v)", *pkb_querier) == expected);
    }

    SECTION("All variables used by a specific assign") {
      std::unordered_set<std::string> expected{"cenX", "cenY"};
      REQUIRE(
          qps_test::RunQuery("variable v; Select v such that Uses(21, v)", *pkb_querier) == expected);
    }
  }

  SECTION("Evaluates to true") {
    SECTION("Print statement modifies a variable") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Uses(9, \"z\")", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("While statement uses a variable") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Uses(5, \"cenX\")", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("If statement uses a variable") {
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Uses(6, \"cenX\")", *pkb_querier)
                  == data[qps::DesignEntity::STMT]);
    }

  }

  SECTION("Evaluates to false") {
    std::unordered_set<std::string> empty{};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Uses(s, \"normSq\")", *pkb_querier)
                == empty);
    REQUIRE(qps_test::RunQuery("if f; Select f such that Uses(6, \"count\")", *pkb_querier)
                == empty);
  }
}

TEST_CASE("QPS can work with different combinations of parent") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddParent({{5, 6}, {5, 11}, {5, 12}, {5, 13}, {5, 14},
                        {6, 7}, {6, 8}, {6, 9}, {6, 10},
                        {15, 16}, {15, 18}, {15, 19}, {15, 20},
                        {16, 17}
                       });


  SECTION("Both Synonyms") {
    SECTION("Look for child statements under while") {
      std::unordered_set<std::string> expected{"6", "11", "12", "13", "14", "17"};
      REQUIRE(qps_test::RunQuery("stmt s; while w; Select s such that Parent(w, s)", *pkb_querier) == expected);
    }

    SECTION("Look for child statements under if") {
      std::unordered_set<std::string> expected{"7", "8", "9", "10", "16", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; if f; Select s such that Parent(f, s)", *pkb_querier) == expected);
    }

    SECTION("Look for child statements under any container stmt") {
      std::unordered_set<std::string>
          expected{"6", "11", "12", "13", "14", "17", "7", "8", "9", "10", "16", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s, s1; Select s such that Parent(s1, s)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under any container stmt") {
      std::unordered_set<std::string> expected{"8", "14"};
      REQUIRE(qps_test::RunQuery("stmt s1; call c; Select c such that Parent(s1, c)", *pkb_querier) == expected);
    }

    SECTION("Look for assign statements under any container stmt") {
      std::unordered_set<std::string> expected{"7", "11", "12", "13", "18", "19", "20", "17"};
      REQUIRE(qps_test::RunQuery("stmt s1; assign a; Select a such that Parent(s1, a)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under a while") {
      std::unordered_set<std::string> expected{"14"};
      REQUIRE(qps_test::RunQuery("while w; call c; Select c such that Parent(w, c)", *pkb_querier) == expected);
    }

    SECTION("Look for call statements under a if") {
      std::unordered_set<std::string> expected{"8"};
      REQUIRE(qps_test::RunQuery("if f; call c; Select c such that Parent(f, c)", *pkb_querier) == expected);
    }

    SECTION("Look for statements under a assign") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("assign a; call c; Select c such that Parent(a, c)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All child statements") {
      std::unordered_set<std::string>
          expected{"6", "11", "12", "13", "14", "17", "7", "8", "9", "10", "16", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(_, s)", *pkb_querier) == expected);
    }

    SECTION("All assign child statements") {
      std::unordered_set<std::string> expected{"7", "11", "12", "13", "18", "19", "20", "17"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(_, a)", *pkb_querier) == expected);
    }

    SECTION("All print child statements") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent(_, pn)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("Statement on left") {
      std::unordered_set<std::string> expected{"5", "6", "15", "16"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, _)", *pkb_querier) == expected);
    }

    SECTION("While on left") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, _)", *pkb_querier) == expected);
    }

    SECTION("If on left") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Parent(f, _)", *pkb_querier) == expected);
    }

    SECTION("Non container stmt on left") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(a, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("call c; Select c such that Parent(c, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent(pn, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"5"};
    REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, 6)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("while w; Select w such that Parent(w, 12)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, 14)", *pkb_querier) == expected);

    expected = {"6"};
    REQUIRE(qps_test::RunQuery("if ifs; Select ifs such that Parent(ifs, 7)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("if ifs; Select ifs such that Parent(ifs, 10)", *pkb_querier) == expected);
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent(s, 9)", *pkb_querier) == expected);
  }

  SECTION("Synonym on right, literal on left") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"11", "12", "13"};
    REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(5, a)", *pkb_querier) == expected);

    expected = {"14"};
    REQUIRE(qps_test::RunQuery("call c; Select c such that Parent(5, c)", *pkb_querier) == expected);

    expected = {"18", "19", "20"};
    REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent(15, a)", *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    SECTION("Both wildcards") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Parent(_, _)", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("Both literals") {
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent(6, 7)", *pkb_querier)
                  == data[qps::DesignEntity::PROCEDURE]);

      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent(5, 11)", *pkb_querier)
                  == data[qps::DesignEntity::CONSTANT]);
    }
  }

  SECTION("Evaluates to false") {
    SECTION("Both literals") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent(10, 9)", *pkb_querier)
                  == empty);
      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent(5, 4)", *pkb_querier)
                  == empty);
    }
  }
}

TEST_CASE("QPS can work with different combinations of parentStar") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddParentStar({{5, 6}, {5, 11}, {5, 12}, {5, 13}, {5, 14},
                            {6, 7}, {6, 8}, {6, 9}, {6, 10},
                            {15, 16}, {15, 18}, {15, 19}, {15, 20},
                            {16, 17}, {5, 7}, {5, 8}, {5, 9}, {5, 10}, {15, 17}
                           });


  SECTION("Both Synonyms") {
    SECTION("Look for nested child statements under while") {
      std::unordered_set<std::string> expected{"6", "7", "8", "9", "10", "11", "12", "13", "14", "17"};
      REQUIRE(qps_test::RunQuery("stmt s; while w; Select s such that Parent*(w, s)", *pkb_querier) == expected);
    }

    SECTION("Look for nested child statements under if") {
      std::unordered_set<std::string> expected{"7", "8", "9", "10", "16", "17", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; if f; Select s such that Parent*(f, s)", *pkb_querier) == expected);
    }

    SECTION("Look for nested child statements under any container stmt") {
      std::unordered_set<std::string>
          expected{"6", "11", "12", "13", "14", "17", "7", "8", "9", "10", "16", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s, s1; Select s such that Parent*(s1, s)", *pkb_querier) == expected);
    }

    SECTION("Look for call nested statements under while") {
      std::unordered_set<std::string> expected{"8", "14"};
      REQUIRE(qps_test::RunQuery("while w; call c; Select c such that Parent*(w, c)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All nested child statements") {
      std::unordered_set<std::string>
          expected{"6", "11", "12", "13", "14", "17", "7", "8", "9", "10", "16", "18", "19", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent*(_, s)", *pkb_querier) == expected);
    }

    SECTION("All assign nested child statements") {
      std::unordered_set<std::string> expected{"7", "11", "12", "13", "18", "19", "20", "17"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent*(_, a)", *pkb_querier) == expected);
    }

    SECTION("All print nested child statements") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent*(_, pn)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("Statement on left") {
      std::unordered_set<std::string> expected{"5", "6", "15", "16"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent*(s, _)", *pkb_querier) == expected);
    }

    SECTION("While on left") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Parent*(w, _)", *pkb_querier) == expected);
    }

    SECTION("If on left") {
      std::unordered_set<std::string> expected{"6", "15"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Parent*(f, _)", *pkb_querier) == expected);
    }

    SECTION("Non container stmt on left") {
      std::unordered_set<std::string> expected{};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent*(a, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("call c; Select c such that Parent*(c, _)", *pkb_querier) == expected);
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Parent*(pn, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"5", "6"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent*(s, 7)", *pkb_querier) == expected);

    expected = {"15", "16"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Parent*(s, 17)", *pkb_querier) == expected);
  }

  SECTION("Synonym on right, literal on left") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"7", "11", "12", "13"};
    REQUIRE(qps_test::RunQuery("assign a; Select a such that Parent*(5, a)", *pkb_querier) == expected);

    expected = {"8", "14"};
    REQUIRE(qps_test::RunQuery("call c; Select c such that Parent*(5, c)", *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    SECTION("Both wildcards") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Parent*(_, _)", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("Both literals") {
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent*(5, 7)", *pkb_querier)
                  == data[qps::DesignEntity::PROCEDURE]);

      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent*(15, 17)", *pkb_querier)
                  == data[qps::DesignEntity::CONSTANT]);
    }
  }

  SECTION("Evaluates to false") {
    SECTION("Both literals") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Parent*(10, 9)", *pkb_querier)
                  == empty);
      REQUIRE(qps_test::RunQuery("constant c; Select c such that Parent*(5, 4)", *pkb_querier)
                  == empty);
    }
  }
}

TEST_CASE("QPS can work with different combinations of follows") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddFollows({{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 15}, {15, 21},
                         {6, 11}, {11, 12}, {12, 13}, {13, 14},
                         {7, 8}, {8, 9},
                         {16, 18}, {19, 20}
                        });


  SECTION("Both Synonyms") {
    SECTION("Look for calls that follow assigns") {
      std::unordered_set<std::string> expected{"4", "8", "14"};
      REQUIRE(qps_test::RunQuery("call c; assign a; Select c such that Follows(a, c)", *pkb_querier) == expected);
    }

    SECTION("Look for stmts that follows another stmts") {
      std::unordered_set<std::string>
          expected{"2", "3", "4", "5", "15", "21", "11", "12", "13", "14", "8", "9", "18", "20"};
      REQUIRE(qps_test::RunQuery("stmt s, f; Select s such that Follows(f, s)", *pkb_querier) == expected);
    }

    SECTION("Look for assigns that followed by another assigns") {
      std::unordered_set<std::string> expected{"1", "2", "11", "12", "19"};
      REQUIRE(qps_test::RunQuery("assign a, b; Select a such that Follows(a, b)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All stmts that follows another stmts") {
      std::unordered_set<std::string>
          expected{"2", "3", "4", "5", "15", "21", "11", "12", "13", "14", "8", "9", "18", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows(_, s)", *pkb_querier) == expected);
    }

    SECTION("All assigns follow stmt") {
      std::unordered_set<std::string> expected{"2", "3", "21", "11", "12", "13", "18", "20"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Follows(_, a)", *pkb_querier) == expected);
    }

    SECTION("All print follow stmt") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Follows(_, pn)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("Statement on left") {
      std::unordered_set<std::string>
          expected{{"1", "2", "3", "4", "5", "15", "6", "11", "12", "13", "7", "8", "16", "19"}};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows(s, _)", *pkb_querier) == expected);
    }

    SECTION("While on left") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Follows(w, _)", *pkb_querier) == expected);
    }

    SECTION("If on left") {
      std::unordered_set<std::string> expected{"15", "6"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Follows(f, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"7"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows(s, 8)", *pkb_querier) == expected);

    expected = {"16"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows(s, 18)", *pkb_querier) == expected);
  }

  SECTION("Synonym on right, literal on left") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"15"};
    REQUIRE(qps_test::RunQuery("if f; Select f such that Follows(5, f)", *pkb_querier) == expected);

    expected = {"9"};
    REQUIRE(qps_test::RunQuery("print pn; Select pn such that Follows(8, pn)", *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    SECTION("Both wildcards") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Follows(_, _)", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("Both literals") {
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Follows(5, 15)", *pkb_querier)
                  == data[qps::DesignEntity::PROCEDURE]);

      REQUIRE(qps_test::RunQuery("constant c; Select c such that Follows(19, 20)", *pkb_querier)
                  == data[qps::DesignEntity::CONSTANT]);
    }
  }

  SECTION("Evaluates to false") {
    SECTION("Both literals") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Follows(10, 9)", *pkb_querier)
                  == empty);
      REQUIRE(qps_test::RunQuery("constant c; Select c such that Follows(5, 4)", *pkb_querier)
                  == empty);
    }
  }
}

TEST_CASE("QPS can work with different combinations of followsStar") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  pkb_helper.AddFollowsStar({{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 15}, {15, 21},
                             {6, 11}, {11, 12}, {12, 13}, {13, 14},
                             {7, 8}, {8, 9},
                             {16, 18}, {19, 20}, {1, 3}, {1, 4}, {1, 5}, {1, 15}, {1, 21},
                             {2, 4}, {2, 5}, {2, 15}, {2, 21}, {3, 5}, {3, 15}, {3, 21},
                             {4, 15}, {4, 21}, {5, 21}, {7, 9},
                             {6, 12}, {6, 13}, {6, 14}, {11, 13}, {11, 14}, {12, 14}
                            });


  SECTION("Both Synonyms") {
    SECTION("Look for calls that followsT assigns") {
      std::unordered_set<std::string> expected{"4", "8", "14"};
      REQUIRE(qps_test::RunQuery("call c; assign a; Select c such that Follows*(a, c)", *pkb_querier) == expected);
    }

    SECTION("Look for stmts that followsT another stmts") {
      std::unordered_set<std::string>
          expected{"2", "3", "4", "5", "15", "21", "11", "12", "13", "14", "8", "9", "18", "20"};
      REQUIRE(qps_test::RunQuery("stmt s, f; Select s such that Follows*(f, s)", *pkb_querier) == expected);
    }

    SECTION("Look for assigns that followsTed by another assigns") {
      std::unordered_set<std::string> expected{"1", "2", "3", "11", "12", "19"};
      REQUIRE(qps_test::RunQuery("assign a, b; Select a such that Follows*(a, b)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All stmts that followsT another stmts") {
      std::unordered_set<std::string>
          expected{"2", "3", "4", "5", "15", "21", "11", "12", "13", "14", "8", "9", "18", "20"};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows*(_, s)", *pkb_querier) == expected);
    }

    SECTION("All assigns follow stmt") {
      std::unordered_set<std::string> expected{"2", "3", "21", "11", "12", "13", "18", "20"};
      REQUIRE(qps_test::RunQuery("assign a; Select a such that Follows*(_, a)", *pkb_querier) == expected);
    }

    SECTION("All print follow stmt") {
      std::unordered_set<std::string> expected{"9"};
      REQUIRE(qps_test::RunQuery("print pn; Select pn such that Follows*(_, pn)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("Statement on left") {
      std::unordered_set<std::string>
          expected{{"1", "2", "3", "4", "5", "15", "6", "11", "12", "13", "7", "8", "16", "19"}};
      REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows*(s, _)", *pkb_querier) == expected);
    }

    SECTION("While on left") {
      std::unordered_set<std::string> expected{"5", "16"};
      REQUIRE(qps_test::RunQuery("while w; Select w such that Follows*(w, _)", *pkb_querier) == expected);
    }

    SECTION("If on left") {
      std::unordered_set<std::string> expected{"15", "6"};
      REQUIRE(qps_test::RunQuery("if f; Select f such that Follows*(f, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"15", "1", "2", "3", "4", "5"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows*(s, 21)", *pkb_querier) == expected);

    expected = {"13", "6", "11", "12"};
    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Follows*(s, 14)", *pkb_querier) == expected);
  }

  SECTION("Synonym on right, literal on left") {
    std::unordered_set<std::string> empty{};
    std::unordered_set<std::string> expected{"15"};
    REQUIRE(qps_test::RunQuery("if f; Select f such that Follows*(2, f)", *pkb_querier) == expected);

    expected = {"9"};
    REQUIRE(qps_test::RunQuery("print pn; Select pn such that Follows*(7, pn)", *pkb_querier) == expected);
  }

  SECTION("Evaluates to true") {
    SECTION("Both wildcards") {
      REQUIRE(qps_test::RunQuery("variable v; Select v such that Follows*(_, _)", *pkb_querier)
                  == data[qps::DesignEntity::VARIABLE]);
    }

    SECTION("Both literals") {
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Follows*(1, 15)", *pkb_querier)
                  == data[qps::DesignEntity::PROCEDURE]);

      REQUIRE(qps_test::RunQuery("constant c; Select c such that Follows*(6, 14)", *pkb_querier)
                  == data[qps::DesignEntity::CONSTANT]);
    }
  }

  SECTION("Evaluates to false") {
    SECTION("Both literals") {
      std::unordered_set<std::string> empty{};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Follows*(1, 6)", *pkb_querier)
                  == empty);
      REQUIRE(qps_test::RunQuery("constant c; Select c such that Follows*(15, 17)", *pkb_querier)
                  == empty);
    }
  }
}

TEST_CASE("QPS can work with different combinations of calls") {
  qps_test::PopulatePKBHelper pkb_helper;
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  // proc1 -> proc2, proc3
  // proc2 -> proc4
  //proc 3-> proc4, proc5
  pkb_helper.AddProcedures({"proc1", "proc2", "proc3", "proc4", "proc5"});
  pkb_helper.AddCallsR({{"proc1", "proc2"}, {"proc1", "proc3"},
                        {"proc2", "proc4"},
                        {"proc3", "proc4"}, {"proc3", "proc5"}
                       });

  SECTION("Both Synonyms") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string>
          expected{"proc1", "proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p, p1; Select p such that Calls(p, p1)", *pkb_querier) == expected);
    }

    SECTION("All procedures being called") {
      std::unordered_set<std::string>
          expected{"proc2", "proc3", "proc4", "proc5"};
      REQUIRE(
          qps_test::RunQuery("procedure p, p1; Select p1 such that Calls(p, p1)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string> expected{"proc1", "proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls(p, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string> expected{"proc2", "proc3", "proc4", "proc5"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls(_, p)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    SECTION("All procedures calling a specific procedure") {
      std::unordered_set<std::string> expected{"proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls(p, \"proc4\")", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, literal on left") {
    SECTION("All procedures being called by a specific procedure") {
      std::unordered_set<std::string> expected{"proc4", "proc5"};
      REQUIRE(
          qps_test::RunQuery("procedure p; Select p such that Calls(\"proc3\", p)", *pkb_querier)
              == expected);
    }
  }
}

TEST_CASE("QPS can work with different combinations of callsStar") {
  qps_test::PopulatePKBHelper pkb_helper;
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  // proc1 -> proc2, proc3
  // proc2 -> proc4
  //proc 3-> proc4, proc5
  pkb_helper.AddProcedures({"proc1", "proc2", "proc3", "proc4", "proc5"});
  pkb_helper.AddCallsStar({{"proc1", "proc2"}, {"proc1", "proc3"}, {"proc1", "proc4"}, {"proc1", "proc5"},
                           {"proc2", "proc4"},
                           {"proc3", "proc4"}, {"proc3", "proc5"}
                          });

  SECTION("Both Synonyms") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string>
          expected{"proc1", "proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p, p1; Select p such that Calls*(p, p1)", *pkb_querier) == expected);
    }

    SECTION("All procedures being called") {
      std::unordered_set<std::string>
          expected{"proc2", "proc3", "proc4", "proc5"};
      REQUIRE(
          qps_test::RunQuery("procedure p, p1; Select p1 such that Calls*(p, p1)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, wild card on right") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string> expected{"proc1", "proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls*(p, _)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, wild card on left") {
    SECTION("All procedures calling") {
      std::unordered_set<std::string> expected{"proc2", "proc3", "proc4", "proc5"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls*(_, p)", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on left, literal on right") {
    SECTION("All procedures calling a specific procedure") {
      std::unordered_set<std::string> expected{"proc1", "proc2", "proc3"};
      REQUIRE(qps_test::RunQuery("procedure p; Select p such that Calls*(p, \"proc4\")", *pkb_querier) == expected);
    }
  }

  SECTION("Synonym on right, literal on left") {
    SECTION("All procedures being called by a specific procedure") {
      std::unordered_set<std::string> expected{"proc2", "proc3", "proc4", "proc5"};
      REQUIRE(
          qps_test::RunQuery("procedure p; Select p such that Calls*(\"proc1\", p)", *pkb_querier)
              == expected);
    }
  }
}


TEST_CASE("QPS can output other synoynm output/failures other than procedure from calls and calls*") {
  qps_test::PopulatePKBHelper pkb_helper;
  auto data = PopulateEntities(pkb_helper);
  QueryFacade* pkb_querier = pkb_helper.GetQuerier();

  // proc1 -> proc2, proc3
  // proc2 -> proc4
  //proc 3-> proc4, proc5
  pkb_helper.AddProcedures({ "proc1", "proc2", "proc3", "proc4", "proc5" });
  pkb_helper.AddCallsR({ {"proc1", "proc2"}, {"proc1", "proc3"},
                        {"proc2", "proc4"},
                        {"proc3", "proc4"}, {"proc3", "proc5"}
                       });
  pkb_helper.AddCallsStar({ {"proc1", "proc2"}, {"proc1", "proc3"},
                      {"proc2", "proc4"},
                      {"proc3", "proc4"}, {"proc3", "proc5"},
                      {"proc1", "proc4"}, {"proc1", "proc5"}
                      });
  //data[qps::DesignEntity::VARIABLE] = { "count", "cenX", "cenY", "flag", "x", "y", "z", "normSq" };
  //data[qps::DesignEntity::CONSTANT] = { "0", "1", "2" };
  //data[qps::DesignEntity::ASSIGN] = { "1", "2", "3", "7", "11", "12", "13", "17", "18", "19", "20", "21" };
  //data[qps::DesignEntity::READ] = { "10" };
  //data[qps::DesignEntity::CALL] = { "4", "8", "14" };
  //data[qps::DesignEntity::IF] = { "6", "15" };
  //data[qps::DesignEntity::WHILE] = { "5", "16" };
  //data[qps::DesignEntity::PRINT] = { "9" };
  //data[qps::DesignEntity::PROCEDURE] = { "computeCentroid", "readPoint" };

  //data[qps::DesignEntity::STMT] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12",
  //                                 "13", "14", "15", "16", "17", "18", "19", "20", "21" };

  //reference for what is inside data

  SECTION("Fail cases where calls/calls* evaluate to false") {
    SECTION("Fail case for call with non-existent procedure as variable name") {
      std::unordered_set<std::string>
        expected{ };
      REQUIRE(qps_test::RunQuery("procedure p, p1; Select p such that Calls(\"fail\", p1)", *pkb_querier) == expected);
    }

    SECTION("Fail case for calls*") {
      std::unordered_set<std::string>
        expected{ };
      REQUIRE(
        qps_test::RunQuery("procedure p, p1; Select p1 such that Calls*(p, \"computeCentroid\")", *pkb_querier) == expected);
    }
  }

  SECTION("Test with return different synonym type") {
    SECTION("Return while, print, read with calls") {
      std::unordered_set<std::string>
        expected{ "5", "16" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select w such that Calls(p, p1)", *pkb_querier) == expected);
      expected = { "10" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select r such that Calls(p, p1)", *pkb_querier) == expected);
      expected = { "9" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select pr such that Calls(p, p1)", *pkb_querier) == expected);
    }

    SECTION("Return while, print, read with calls*") {
      std::unordered_set<std::string>
        expected{ "5", "16" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select w such that Calls*(p, p1)", *pkb_querier) == expected);
      expected = { "10" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select r such that Calls*(p, p1)", *pkb_querier) == expected);
      expected = { "9" };
      REQUIRE(qps_test::RunQuery("while w; read r; print pr; procedure p, p1; Select pr such that Calls*(p, p1)", *pkb_querier) == expected);
    }

    SECTION("Return other procedures not involved") {
      SECTION("Procedures that are not part of calls") {
        std::unordered_set<std::string> expected{ "computeCentroid", "readPoint", "proc1", "proc2" , "proc3" , "proc4" , "proc5" };
        REQUIRE(qps_test::RunQuery("procedure p, p1, p2; Select p2 such that Calls(p, p1)", *pkb_querier) == expected);
      }

      SECTION("Procedures that are not part of calls*") {
        std::unordered_set<std::string> expected{ "computeCentroid", "readPoint", "proc1", "proc2" , "proc3" , "proc4" , "proc5" };
        REQUIRE(qps_test::RunQuery("procedure p, p1, p2; Select p2 such that Calls*(p, p1)", *pkb_querier) == expected);
      }
    }
  }
}
