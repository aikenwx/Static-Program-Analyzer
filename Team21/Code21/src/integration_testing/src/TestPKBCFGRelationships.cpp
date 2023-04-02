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

  auto test = NextRelationship::getRelationshipTypeStatic();
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

    auto *temp = pkb_querier->getRelationshipsByTypes(NextStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());
    //

    REQUIRE(temp->size() == 100);

    auto *temp2 = pkb_querier->getRelationshipsByTypes(NextStarRelationship::getRelationshipTypeStatic(), AssignStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

    REQUIRE(temp2->size() == 47);

//    auto *temp3 = pkb_querier->getRelationshipsByTypes(AffectsStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());
//
//    REQUIRE(temp3->size() == 27);

    auto *temp4 = pkb_querier->getRelationshipsByTypes(AffectsStarRelationship::getRelationshipTypeStatic(), ReadStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

    REQUIRE(temp4->empty());

    auto *temp5 = pkb_querier->getRelationshipsByTypes(AffectsStarRelationship::getRelationshipTypeStatic(), AssignStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

//    REQUIRE(temp5->size() == 27);

    //
    auto pairVector = std::vector<std::pair<std::string, std::string>>();
    //
    for (auto *relationship : *temp5) {
      auto val1 = *relationship->getLeftHandEntity()->getEntityValue();
      auto val2 = *relationship->getRightHandEntity()->getEntityValue();
      pairVector.emplace_back(val1, val2);
    }
    // 11 12, 9 10, 9 11, 9 12, 6 10, 6 11, 6 12, 6 6, 10 11, 10 12, 8 10, 8 11, 8 12, 4 10, 4 11, 4 12,4 8, 4 4,2 10, 2 11, 2 12, 2 6, 1 10, 1 11, 1 12, 1 8, 1 4

    REQUIRE(pairVector.size() == 27);

    auto expectedPairs = std::vector<std::pair<std::string, std::string>>({{"11", "12"}, {"9", "10"}, {"9", "11"}, {"9", "12"}, {"6", "10"}, {"6", "11"}, {"6", "12"}, {"6", "6"}, {"10", "11"}, {"10", "12"}, {"8", "10"}, {"8", "11"}, {"8", "12"}, {"4", "10"}, {"4", "11"}, {"4", "12"}, {"4", "8"}, {"4", "4"}, {"2", "10"}, {"2", "11"}, {"2", "12"}, {"2", "6"}, {"1", "10"}, {"1", "11"}, {"1", "12"}, {"1", "8"}, {"1", "4"}});

    for (const auto &pair : expectedPairs) {
      REQUIRE(std::find(pairVector.begin(), pairVector.end(), pair) != pairVector.end());
    }

    auto *temp6 = pkb_querier->getRelationshipsByTypes(AffectsStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

    REQUIRE(temp6->size() == 27);

    auto *temp7 = pkb_querier->getRelationshipsByTypes(AffectsRelationship::getRelationshipTypeStatic(), AssignStatement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

    REQUIRE(temp7->size() == 18);
    auto *temp8 = pkb_querier->getRelationshipsByLeftEntityLiteralAndRightEntityType(AffectsStarRelationship::getRelationshipTypeStatic(), Statement::getEntityTypeStatic(), 1, Statement::getEntityTypeStatic());

    REQUIRE(temp8->size() == 5);

    auto *temp9 = pkb_querier->getRelationshipsByLeftEntityTypeAndRightEntityLiteral(NextStarRelationship::getRelationshipTypeStatic(), CallStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic(), 8);

    REQUIRE(temp9->size() == 1);
    REQUIRE(*temp9->at(0)->getEntityValue() == "5");

    auto *temp10 = pkb_querier->getRelationshipsByTypes(NextStarRelationship::getRelationshipTypeStatic(), CallStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());
    REQUIRE(temp10->size() == 12);

    auto *temp11 = pkb_querier->getRelationshipsByLeftEntityTypeAndRightEntityLiteral(
        NextStarRelationship::getRelationshipTypeStatic(),
        CallStatement::getEntityTypeStatic(),
        Statement::getEntityTypeStatic(),
        8);

    auto *temp12 = pkb_querier->getRelationship(
        NextStarRelationship::getRelationshipTypeStatic(),
        CallStatement::getEntityTypeStatic(),
        5,
        Statement::getEntityTypeStatic(),
        8);

    auto *temp13 = pkb_querier->getRelationship(
        NextStarRelationship::getRelationshipTypeStatic(),
        Statement::getEntityTypeStatic(),
        5,
        Statement::getEntityTypeStatic(),
        8);

    REQUIRE(*temp11->at(0)->getEntityValue() == "5");
    REQUIRE(*temp12->getLeftHandEntity()->getEntityValue() == "5");
    REQUIRE(*temp13->getLeftHandEntity()->getEntityValue() == "5");
  }
}

TEST_CASE("Test relationship retrieval with Statement entity type") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count"};
  data[qps::DesignEntity::ASSIGN] = {"1"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  pkb_helper.AddStatementModifies({{1, "count"}});
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();
  auto *relationship = pkb_querier->getRelationship(ModifiesRelationship::getRelationshipTypeStatic(),
                                                    Statement::getEntityTypeStatic(),
                                                    1,
                                                    Variable::getEntityTypeStatic(),
                                                    "count");
  REQUIRE(relationship != nullptr);
}

TEST_CASE("Test incorrect/premature caching") {
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
  pkb_helper.StoreCFG(cfg);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  auto *temp1 = pkb_querier->getRelationshipsByTypes(NextRelationship::getRelationshipTypeStatic(),
                                                     WhileStatement::getEntityTypeStatic(),
                                                     Statement::getEntityTypeStatic());

  REQUIRE(temp1->size() == 2);

  auto *temp3 = pkb_querier->getRelationshipsByTypes(NextRelationship::getRelationshipTypeStatic(),
                                                     Statement::getEntityTypeStatic(),
                                                     Statement::getEntityTypeStatic());

  REQUIRE(temp3->size() == 13);
}

TEST_CASE("Test processed sp") {
  auto pkb = PKB();

  const auto *program = R"(procedure main {
  while (x + y < f - 5) {
    if (x > 5) then {
      call f;
    }
    else {
      call g;
    }
    x = y + 5 + x;

    if (1 == 1) then {
      print z;
    } else {
      print x;
    }
  }
}

procedure f {
  if (x > 5) then {
    call g;
  }
  else {
    read x;
  }
}

procedure g {
  print z;
  z = z + 1;
  print x;

  if (x + y > 5) then {
    while (z < 5) {
      read x;
      print x;
    }
    while (10 % 5 >= 5) {
      print x;
    }
  } else {
    if (x + y > 5 + x) then {
      x = x + 1 + y;
    } else {
      y = y + 1;
      z = z + 1;
      x = y + 1;
    }
  }

  while (x + y < 1) {
    read x;
  }
})";

  sp::SP::process(program, &pkb);

  auto *result = pkb.getQueryFacade()->getRelationshipsByTypes(NextStarRelationship::getRelationshipTypeStatic(),
                                                               AssignStatement::getEntityTypeStatic(),
                                                               CallStatement::getEntityTypeStatic());

  REQUIRE(result->size() == 2);

  REQUIRE(*result->at(0)->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*result->at(0)->getRightHandEntity()->getEntityValue() == "3");
  REQUIRE(*result->at(1)->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*result->at(1)->getRightHandEntity()->getEntityValue() == "4");
}

TEST_CASE("Retrieve related entities to non existent entity") {
  qps_test::PopulatePKBHelper pkb_helper;
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  auto *relationship =
      pkb_querier->getRelationshipsByLeftEntityLiteralAndRightEntityType(CallsRelationship::getRelationshipTypeStatic(),
                                                                         Procedure::getEntityTypeStatic(),
                                                                         "fail",
                                                                         Procedure::getEntityTypeStatic());
  REQUIRE(relationship == RelationshipManager::getEmptyEntityVector());
}

TEST_CASE("Test Affects/Affects*") {
  auto pkb = PKB();

  const auto *program = R"(procedure test {
 y = x / 2;
 if (y < 5) then {
  z = y;
  print z;
  while (y < 5) {
   x = x - y;
   y = y + 1;
   z = x;
  }
  y = x + z;
 } else {
  x = y;
 }
 x = x + y;
 v = y + 2;
 y = v + x;
})";

  sp::SP::process(program, &pkb);

  auto *result = pkb.getQueryFacade()->getRelationshipsByLeftEntityLiteralAndRightEntityType(AffectsRelationship::getRelationshipTypeStatic(),
                                                                                             Statement::getEntityTypeStatic(), 6,
                                                                                             Statement::getEntityTypeStatic());

  REQUIRE(result->size() == 4);
}

}  // namespace qps

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
