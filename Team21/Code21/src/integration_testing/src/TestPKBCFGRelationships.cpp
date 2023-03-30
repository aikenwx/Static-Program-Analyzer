#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "PKBStorageClasses/RelationshipClasses/AffectsRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/AffectsStarRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/NextRelationship.h"
#include "PKBStorageClasses/RelationshipClasses/NextStarRelationship.h"
#include "PopulatePKBHelper.cpp"
#include "QPSUtilities.h"
#include "sp/sp.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

namespace qps {
// procedure Second {
//     1.      x = 0;
//     2.      i = 5;
//     3.      while (i!=0)  {
//       4.          x = x + 2*y;
//       5.          call Third;
//       6.          i = i - 1;
//     }
//     7.      if (x==1) then {
//       8.         x = x+1;
//     } else {
//       9.        z = 1;
//     }
//     10.     z = z + x + i;
//     11.     y = z + 2;
//     12.     x = x * y + z;
//     13.     read x
//     14.     y = x;
// }
TEST_CASE("Next/Affect/Affect* Clauses PKB") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "4", "6", "8", "9", "10", "11", "12", "14"};
  data[qps::DesignEntity::WHILE] = {"3"};
  data[qps::DesignEntity::IF] = {"7"};
  data[qps::DesignEntity::CALL] = {"5"};
  data[qps::DesignEntity::VARIABLE] = {"x", "y", "z", "i"};
  data[qps::DesignEntity::READ] = {"13"};

  auto cfg = std::make_shared<cfg::CFG>();
  auto one = std::make_shared<cfg::Block>(1, 2);
  auto three = std::make_shared<cfg::Block>(3, 3);
  auto four = std::make_shared<cfg::Block>(4, 6);
  auto seven = std::make_shared<cfg::Block>(7, 7);
  auto eight = std::make_shared<cfg::Block>(8, 8);
  auto nine = std::make_shared<cfg::Block>(9, 9);
  auto ten = std::make_shared<cfg::Block>(10, 14);

  one->AddChild(three);
  three->AddChild(four);
  three->AddChild(seven);
  four->AddChild(three);
  seven->AddChild(eight);
  seven->AddChild(nine);
  eight->AddChild(ten);
  nine->AddChild(ten);

  cfg->InsertBlock(one);
  cfg->InsertBlock(three);
  cfg->InsertBlock(four);
  cfg->InsertBlock(seven);
  cfg->InsertBlock(eight);
  cfg->InsertBlock(nine);
  cfg->InsertBlock(ten);

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  pkb_helper.AddStatementModifies({{1, "x"}, {2, "i"}, {4, "x"}, {6, "i"}, {8, "x"}, {9, "z"}, {10, "z"}, {11, "y"}, {12, "x"}, {13, "x"}, {14, "y"}});
  pkb_helper.AddStatementUses({{3, "i"}, {4, "x"}, {4, "y"}, {6, "i"}, {8, "x"}, {10, "z"}, {10, "x"}, {10, "i"}, {11, "z"}, {12, "x"}, {12, "y"}, {12, "z"}, {14, "x"}});
  pkb_helper.StoreCFG(cfg);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  SECTION("Next/Affect/Affect*: (LIT, LIT)") {
    REQUIRE(pkb_querier->getRelationship(NextRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 3, Statement::getEntityTypeStatic(),
                                         4) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         7, Statement::getEntityTypeStatic(), 8) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         7, Statement::getEntityTypeStatic(), 9) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         7, Statement::getEntityTypeStatic(), 9) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         1, Statement::getEntityTypeStatic(), 12) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         1, Statement::getEntityTypeStatic(), 9) != nullptr);

    REQUIRE(pkb_querier->getRelationship(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         8, Statement::getEntityTypeStatic(), 9) == nullptr);

    REQUIRE(pkb_querier->getRelationship(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(),
                                         6, Statement::getEntityTypeStatic(), 4) != nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         8) != nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsStarRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 8, Statement::getEntityTypeStatic(),
                                         12) != nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsStarRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         12) != nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsStarRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         6) == nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         6) == nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         11) == nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsStarRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         12) != nullptr);

    REQUIRE(pkb_querier->getRelationship(AffectsStarRelationship::getRelationshipTypeStatic(),
                                         Statement::getEntityTypeStatic(), 4, Statement::getEntityTypeStatic(),
                                         14) == nullptr);
  }
}
}  // namespace qps

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
