#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"

namespace qps {
//procedure Second {
//    1.      x = 0;
//    2.      i = 5;
//    3.      while (i!=0)  {
//      4.          x = x + 2*y;
//      5.          call Third;
//      6.         i = i - 1;
//    }
//    7.      if (x==1) then {
//      8.         x = x+1;
//    } else {
//      9.        z = 1;
//    }
//    10.     z = z + x + i;
//    11.     y = z + 2;
//    12.     x = x * y + z;
//}


TEST_CASE("Next Clauses") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "4", "6", "8", "9", "10", "11", "12"};
  data[qps::DesignEntity::WHILE] = {"3"};
  data[qps::DesignEntity::IF] = {"7"};
  data[qps::DesignEntity::CALL] = {"5"};

  auto cfg = std::make_shared<cfg::CFG>();
  auto one = std::make_shared<cfg::Block>(1, 2);
  auto three = std::make_shared<cfg::Block>(3, 3);
  auto four = std::make_shared<cfg::Block>(4, 6);
  auto seven = std::make_shared<cfg::Block>(7, 7);
  auto eight = std::make_shared<cfg::Block>(8, 8);
  auto nine = std::make_shared<cfg::Block>(9, 9);
  auto ten = std::make_shared<cfg::Block>(10, 12);

  one->AddChild(three);
  three->AddChild(four);
  three->AddChild(seven);
  four->AddChild(three);
//  seven->AddChild(eight);
//  seven->AddChild(nine);
//  eight->AddChild(ten);
//  nine->AddChild(ten);

  three->AddParent(one);
  four->AddParent(three);
  seven->AddParent(three);
  three->AddParent(four);
//  eight->AddParent(seven);
//  nine->AddParent(seven);
//  ten->AddParent(eight);
//  ten->AddParent(nine);

  cfg->InsertBlock(one);
  cfg->InsertBlock(three);
  cfg->InsertBlock(four);
  cfg->InsertBlock(seven);
//  cfg->InsertBlock(eight);
//  cfg->InsertBlock(nine);
//  cfg->InsertBlock(ten);

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  pkb_helper.StoreCFG(cfg);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

//  SECTION("Next: (LIT, LIT)") {
//    std::unordered_set<std::string> TRUE{"TRUE"};
//    std::unordered_set<std::string> FALSE{"FALSE"};
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(3,4)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(3,7)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(5,6)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(6,3)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(8,10)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(9,10)", *pkb_querier) == TRUE);
//
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(4, 3)", *pkb_querier) == FALSE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(12, 13)", *pkb_querier) == FALSE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next(1, 3)", *pkb_querier) == FALSE);
//  }

//  SECTION("Next: (LIT, SYN)") {
//    std::unordered_set<std::string> expected{"4", "7"};
//    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Next(3,s)", *pkb_querier) == expected);
//
//    expected = {"7"};
//    REQUIRE(qps_test::RunQuery("if f; Select f such that Next(3,f)", *pkb_querier) == expected);
//
//    expected = {"8", "9"};
//    REQUIRE(qps_test::RunQuery("stmt s; Select s such that Next(7,s)", *pkb_querier) == expected);
//  }

//  SECTION("Next: (SYN, LIT)") {
//    std::unordered_set<std::string> expected{"7"};
//    REQUIRE(qps_test::RunQuery("if f; Select f such that Next(f,8)", *pkb_querier) == expected);
//  }

//  SECTION("Next: (SYN, SYN)") {
//    std::unordered_set<std::string> expected{"3 4", "3 7"};
//    REQUIRE(qps_test::RunQuery("stmt s; while w; Select <w,s> such that Next(w,s)", *pkb_querier) == expected);
//    expected = {"1 2", "2 3", "3 4", "3 7", "4 5", "5 6", "6 3", "7 8", "7 9", "8 10", "9 10",
//                "10 11", "11 12"};
//    REQUIRE(qps_test::RunQuery("stmt s,s1; Select <s,s1> such that Next(s,s1)", *pkb_querier) == expected);
//  }

//  SECTION("NEXT*: (LIT LIT)") {
//    std::unordered_set<std::string> TRUE{"TRUE"};
//    std::unordered_set<std::string> FALSE{"FALSE"};
//
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(3,4)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(3,10)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(4,3)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(3,3)", *pkb_querier) == TRUE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(5,4)", *pkb_querier) == TRUE);
//
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(8,8)", *pkb_querier) == FALSE);
//    REQUIRE(qps_test::RunQuery("Select BOOLEAN such that Next*(8,7)", *pkb_querier) == FALSE);
//  }
//
//  SECTION("NEXT*: (SYN LIT)") {
//    std::unordered_set<std::string> expected{"5"};
//    REQUIRE(qps_test::RunQuery("call c; Select c such that Next*(c, 8)", *pkb_querier) == expected);
//  }
//
//  SECTION("NEXT*: (LIT SYN)") {
//    std::unordered_set<std::string> expected{"4", "6", "8", "9", "10", "11", "12"};
//    REQUIRE(qps_test::RunQuery("assign a; Select a such that Next*(4, a)", *pkb_querier) == expected);
//  }

  SECTION("NEXT*: (SYN SYN)") {
//    std::unordered_set<std::string>
//        expected{"1 2", "1 3", "1 4", "1 5", "1 6", "1 7", "1 8", "1 9", "1 10", "1 11", "1 12",
//                 "2 3", "2 4", "2 5", "2 6", "2 7", "2 8", "2 9", "2 10", "2 11", "2 12",
//                 "3 4", "3 5", "3 6", "3 7", "3 8", "3 9", "3 10", "3 11", "3 12",
//                 "4 5", "4 6", "4 7", "4 8", "4 9", "4 10", "4 11", "4 12", "4 3", "4, 4",
//                 "5 6", "5 7", "5 8", "5 9", "5 10", "5 11", "5 12", "5 4", "5, 3", "5, 5",
//                 "6 6", "6 7", "6 8", "6 9", "6 10", "6 11", "6 12", "6 3", "6 4", "6 5",
//                 "7 8", "7 9", "7 10", "7 11", "7 12",
//                 "8 10", "8 11", "8 12",
//                 "9 10", "9 11", "9 12",
//                 "10 11", "10 12",
//                 "11 12"};
    std::unordered_set<std::string>
        expected{"1 2", "1 3", "1 4", "1 5", "1 6", "1 7",
                 "2 3", "2 4", "2 5", "2 6", "2 7",
                 "3 4", "3 5", "3 6", "3 7",
                 "4 5", "4 6", "4 7", "4, 3", "4, 4",
                 "5 6", "5 7", "5, 3", "5, 4", "5, 5",
                 "6 6", "6 7", "6 3", "6 4", "6 5"};
    auto ans = qps_test::RunQuery("stmt s,s1; Select <s, s1> such that Next*(s, s1)", *pkb_querier);
    std::vector<std::string> a(ans.begin(), ans.end());
    std::vector<std::string> b(expected.begin(), expected.end());
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    REQUIRE(a == b);
//    REQUIRE(qps_test::RunQuery("stmt s,s1; Select <s, s1> such that Next*(s, s1)", *pkb_querier) == expected);
  }
}
} // qps
