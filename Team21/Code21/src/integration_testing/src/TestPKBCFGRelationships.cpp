#include <catch2/catch_test_macros.hpp>
#include <iostream>
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
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "4", "6", "8",
                                     "9", "10", "11", "12", "14"};
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
  pkb_helper.AddStatementModifies({{1, "x"},
                                   {2, "i"},
                                   {4, "x"},
                                   {6, "i"},
                                   {8, "x"},
                                   {9, "z"},
                                   {10, "z"},
                                   {11, "y"},
                                   {12, "x"},
                                   {13, "x"},
                                   {14, "y"}});
  pkb_helper.AddStatementUses({{3, "i"},
                               {4, "x"},
                               {4, "y"},
                               {6, "i"},
                               {8, "x"},
                               {10, "z"},
                               {10, "x"},
                               {10, "i"},
                               {11, "z"},
                               {12, "x"},
                               {12, "y"},
                               {12, "z"},
                               {14, "x"}});
  pkb_helper.StoreCFG(cfg);
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  auto test = NextRelationship::getRelationshipTypeStatic();
  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 3,
              Statement::getEntityTypeStatic(), 4) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 8) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 9) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 9) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 1,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 1,
              Statement::getEntityTypeStatic(), 9) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 8,
              Statement::getEntityTypeStatic(), 9) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 6,
              Statement::getEntityTypeStatic(), 4) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 8) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 8,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 6) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 6) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 11) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 14) == nullptr);

  auto *temp = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());
  //

  REQUIRE(temp->size() == 100);

  auto *temp2 = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp2->size() == 47);

  auto *temp3 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

  REQUIRE(temp3->size() == 27);

  auto *temp4 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      ReadStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp4->empty());

  auto *temp5 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp5->size() == 27);

  auto pairVector = std::vector<std::pair<std::string, std::string>>();

  for (auto *relationship : *temp5) {
    auto val1 = *relationship->getLeftHandEntity()->getEntityValue();
    auto val2 = *relationship->getRightHandEntity()->getEntityValue();
    pairVector.emplace_back(val1, val2);
  }

  REQUIRE(pairVector.size() == 27);

  auto expectedPairs = std::vector<std::pair<std::string, std::string>>(
      {{"11", "12"}, {"9", "10"}, {"9", "11"}, {"9", "12"}, {"6", "10"}, {"6", "11"}, {"6", "12"}, {"6", "6"}, {"10", "11"}, {"10", "12"}, {"8", "10"}, {"8", "11"}, {"8", "12"}, {"4", "10"}, {"4", "11"}, {"4", "12"}, {"4", "8"}, {"4", "4"}, {"2", "10"}, {"2", "11"}, {"2", "12"}, {"2", "6"}, {"1", "10"}, {"1", "11"}, {"1", "12"}, {"1", "8"}, {"1", "4"}});

  for (const auto &pair : expectedPairs) {
    REQUIRE(std::find(pairVector.begin(), pairVector.end(), pair) !=
            pairVector.end());
  }

  auto *temp6 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  REQUIRE(temp6->size() == 27);

  auto *temp7 = pkb_querier->getRelationshipsByTypes(
      AffectsRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp7->size() == 18);
  auto *temp8 =
          pkb_querier->getRightEntitiesRelatedToGivenLeftEntity(
                  AffectsStarRelationship::getRelationshipTypeStatic(),
                  Statement::getEntityTypeStatic(), 1,
                  Statement::getEntityTypeStatic());

  REQUIRE(temp8->size() == 5);

  auto *temp9 =
          pkb_querier->getLeftEntitiesRelatedToGivenRightEntity(
                  NextStarRelationship::getRelationshipTypeStatic(),
                  CallStatement::getEntityTypeStatic(),
                  Statement::getEntityTypeStatic(), 8);

  REQUIRE(temp9->size() == 1);
  REQUIRE(*temp9->at(0)->getEntityValue() == "5");

  auto *temp10 = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      CallStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());
  REQUIRE(temp10->size() == 12);

  auto *temp11 =
          pkb_querier->getLeftEntitiesRelatedToGivenRightEntity(
                  NextStarRelationship::getRelationshipTypeStatic(),
                  CallStatement::getEntityTypeStatic(),
                  Statement::getEntityTypeStatic(), 8);

  auto *temp12 = pkb_querier->getRelationship(
      NextStarRelationship::getRelationshipTypeStatic(),
      CallStatement::getEntityTypeStatic(), 5, Statement::getEntityTypeStatic(),
      8);

  auto *temp13 = pkb_querier->getRelationship(
      NextStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), 5, Statement::getEntityTypeStatic(), 8);

  REQUIRE(*temp11->at(0)->getEntityValue() == "5");
  REQUIRE(*temp12->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*temp13->getLeftHandEntity()->getEntityValue() == "5");
}

TEST_CASE("Test relationship retrieval with Statement entity type") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::VARIABLE] = {"count"};
  data[qps::DesignEntity::ASSIGN] = {"1"};

  qps_test::PopulatePKBHelper pkb_helper;
  pkb_helper.PopulateEntities(data);
  pkb_helper.AddStatementModifies({{1, "count"}});
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();
  auto *relationship = pkb_querier->getRelationship(
      ModifiesRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), 1, Variable::getEntityTypeStatic(),
      "count");
  REQUIRE(relationship != nullptr);
}

TEST_CASE("Test incorrect/premature caching") {
  qps_test::PopulatePKBHelper::Data data;
  data[qps::DesignEntity::ASSIGN] = {"1", "2", "4", "6", "8",
                                     "9", "10", "11", "12"};
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

  auto *temp1 = pkb_querier->getRelationshipsByTypes(
      NextRelationship::getRelationshipTypeStatic(),
      WhileStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  REQUIRE(temp1->size() == 2);

  auto *temp3 = pkb_querier->getRelationshipsByTypes(
      NextRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

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

  auto *result = pkb.getQueryFacade()->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      CallStatement::getEntityTypeStatic());

  REQUIRE(result->size() == 2);

  REQUIRE(*result->at(0)->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*result->at(0)->getRightHandEntity()->getEntityValue() == "4");
  REQUIRE(*result->at(1)->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*result->at(1)->getRightHandEntity()->getEntityValue() == "3");
}

TEST_CASE("Retrieve related entities to non existent entity") {
  qps_test::PopulatePKBHelper pkb_helper;
  QueryFacade *pkb_querier = pkb_helper.GetQuerier();

  auto *relationship =
          pkb_querier->getRightEntitiesRelatedToGivenLeftEntity(
                  CallsRelationship::getRelationshipTypeStatic(),
                  Procedure::getEntityTypeStatic(), "fail",
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

  auto *result = pkb.getQueryFacade()
          ->getRightEntitiesRelatedToGivenLeftEntity(
                  AffectsRelationship::getRelationshipTypeStatic(),
                  Statement::getEntityTypeStatic(), 6,
                  Statement::getEntityTypeStatic());

  REQUIRE(result->size() == 4);
}

TEST_CASE("Test reverse Affects") {
  auto pkb = PKB();

  const auto *program = R"(

  procedure f {
    a = 1;
    a = x + y;
  }

  procedure g {
    z = 1;
    call f;
  }

  procedure test {
    while (x < 5) {
      x = 1;
      call f;
      o = x + y + z + a;
      z = 2;
    }
})";

  sp::SP::process(program, &pkb);

  auto *result = pkb.getQueryFacade()
          ->getLeftEntitiesRelatedToGivenRightEntity(
                  AffectsRelationship::getRelationshipTypeStatic(),
                  Statement::getEntityTypeStatic(),
                  Statement::getEntityTypeStatic(), 8);

  REQUIRE(result->size() == 2);
  REQUIRE(*result->at(0)->getEntityValue() == "6");
  REQUIRE(*result->at(1)->getEntityValue() == "9");
}

TEST_CASE("Test AffectsStar worst case") {
  auto pkb = PKB();
  // 500 line simple program where each stmt affects the next stmt, within a
  // while loop
  const auto *program = R"(
  procedure main {
    while (x == y) {
      v1 = v500;
      v2 = v1;
      v3 = v2;
      v4 = v3;
      v5 = v4;
      v6 = v5;
      v7 = v6;
      v8 = v7;
      v9 = v8;
      v10 = v9;
      v11 = v10;
      v12 = v11;
      v13 = v12;
      v14 = v13;
      v15 = v14;
      v16 = v15;
      v17 = v16;
      v18 = v17;
      v19 = v18;
      v20 = v19;
      v21 = v20;
      v22 = v21;
      v23 = v22;
      v24 = v23;
      v25 = v24;
      v26 = v25;
      v27 = v26;
      v28 = v27;
      v29 = v28;
      v30 = v29;
      v31 = v30;
      v32 = v31;
      v33 = v32;
      v34 = v33;
      v35 = v34;
      v36 = v35;
      v37 = v36;
      v38 = v37;
      v39 = v38;
      v40 = v39;
      v41 = v40;
      v42 = v41;
      v43 = v42;
      v44 = v43;
      v45 = v44;
      v46 = v45;
      v47 = v46;
      v48 = v47;
      v49 = v48;
      v50 = v49;
      v51 = v50;
      v52 = v51;
      v53 = v52;
      v54 = v53;
      v55 = v54;
      v56 = v55;
      v57 = v56;
      v58 = v57;
      v59 = v58;
      v60 = v59;
      v61 = v60;
      v62 = v61;
      v63 = v62;
      v64 = v63;
      v65 = v64;
      v66 = v65;
      v67 = v66;
      v68 = v67;
      v69 = v68;
      v70 = v69;
      v71 = v70;
      v72 = v71;
      v73 = v72;
      v74 = v73;
      v75 = v74;
      v76 = v75;
      v77 = v76;
      v78 = v77;
      v79 = v78;
      v80 = v79;
      v81 = v80;
      v82 = v81;
      v83 = v82;
      v84 = v83;
      v85 = v84;
      v86 = v85;
      v87 = v86;
      v88 = v87;
      v89 = v88;
      v90 = v89;
      v91 = v90;
      v92 = v91;
      v93 = v92;
      v94 = v93;
      v95 = v94;
      v96 = v95;
      v97 = v96;
      v98 = v97;
      v99 = v98;
      v100 = v99;
      v101 = v100;
      v102 = v101;
      v103 = v102;
      v104 = v103;
      v105 = v104;
      v106 = v105;
      v107 = v106;
      v108 = v107;
      v109 = v108;
      v110 = v109;
      v111 = v110;
      v112 = v111;
      v113 = v112;
      v114 = v113;
      v115 = v114;
      v116 = v115;
      v117 = v116;
      v118 = v117;
      v119 = v118;
      v120 = v119;
      v121 = v120;
      v122 = v121;
      v123 = v122;
      v124 = v123;
      v125 = v124;
      v126 = v125;
      v127 = v126;
      v128 = v127;
      v129 = v128;
      v130 = v129;
      v131 = v130;
      v132 = v131;
      v133 = v132;
      v134 = v133;
      v135 = v134;
      v136 = v135;
      v137 = v136;
      v138 = v137;
      v139 = v138;
      v140 = v139;
      v141 = v140;
      v142 = v141;
      v143 = v142;
      v144 = v143;
      v145 = v144;
      v146 = v145;
      v147 = v146;
      v148 = v147;
      v149 = v148;
      v150 = v149;
      v151 = v150;
      v152 = v151;
      v153 = v152;
      v154 = v153;
      v155 = v154;
      v156 = v155;
      v157 = v156;
      v158 = v157;
      v159 = v158;
      v160 = v159;
      v161 = v160;
      v162 = v161;
      v163 = v162;
      v164 = v163;
      v165 = v164;
      v166 = v165;
      v167 = v166;
      v168 = v167;
      v169 = v168;
      v170 = v169;
      v171 = v170;
      v172 = v171;
      v173 = v172;
      v174 = v173;
      v175 = v174;
      v176 = v175;
      v177 = v176;
      v178 = v177;
      v179 = v178;
      v180 = v179;
      v181 = v180;
      v182 = v181;
      v183 = v182;
      v184 = v183;
      v185 = v184;
      v186 = v185;
      v187 = v186;
      v188 = v187;
      v189 = v188;
      v190 = v189;
      v191 = v190;
      v192 = v191;
      v193 = v192;
      v194 = v193;
      v195 = v194;
      v196 = v195;
      v197 = v196;
      v198 = v197;
      v199 = v198;
      v200 = v199;
      v201 = v200;
      v202 = v201;
      v203 = v202;
      v204 = v203;
      v205 = v204;
      v206 = v205;
      v207 = v206;
      v208 = v207;
      v209 = v208;
      v210 = v209;
      v211 = v210;
      v212 = v211;
      v213 = v212;
      v214 = v213;
      v215 = v214;
      v216 = v215;
      v217 = v216;
      v218 = v217;
      v219 = v218;
      v220 = v219;
      v221 = v220;
      v222 = v221;
      v223 = v222;
      v224 = v223;
      v225 = v224;
      v226 = v225;
      v227 = v226;
      v228 = v227;
      v229 = v228;
      v230 = v229;
      v231 = v230;
      v232 = v231;
      v233 = v232;
      v234 = v233;
      v235 = v234;
      v236 = v235;
      v237 = v236;
      v238 = v237;
      v239 = v238;
      v240 = v239;
      v241 = v240;
      v242 = v241;
      v243 = v242;
      v244 = v243;
      v245 = v244;
      v246 = v245;
      v247 = v246;
      v248 = v247;
      v249 = v248;
      v250 = v249;
      v251 = v250;
      v252 = v251;
      v253 = v252;
      v254 = v253;
      v255 = v254;
      v256 = v255;
      v257 = v256;
      v258 = v257;
      v259 = v258;
      v260 = v259;
      v261 = v260;
      v262 = v261;
      v263 = v262;
      v264 = v263;
      v265 = v264;
      v266 = v265;
      v267 = v266;
      v268 = v267;
      v269 = v268;
      v270 = v269;
      v271 = v270;
      v272 = v271;
      v273 = v272;
      v274 = v273;
      v275 = v274;
      v276 = v275;
      v277 = v276;
      v278 = v277;
      v279 = v278;
      v280 = v279;
      v281 = v280;
      v282 = v281;
      v283 = v282;
      v284 = v283;
      v285 = v284;
      v286 = v285;
      v287 = v286;
      v288 = v287;
      v289 = v288;
      v290 = v289;
      v291 = v290;
      v292 = v291;
      v293 = v292;
      v294 = v293;
      v295 = v294;
      v296 = v295;
      v297 = v296;
      v298 = v297;
      v299 = v298;
      v300 = v299;
      v301 = v300;
      v302 = v301;
      v303 = v302;
      v304 = v303;
      v305 = v304;
      v306 = v305;
      v307 = v306;
      v308 = v307;
      v309 = v308;
      v310 = v309;
      v311 = v310;
      v312 = v311;
      v313 = v312;
      v314 = v313;
      v315 = v314;
      v316 = v315;
      v317 = v316;
      v318 = v317;
      v319 = v318;
      v320 = v319;
      v321 = v320;
      v322 = v321;
      v323 = v322;
      v324 = v323;
      v325 = v324;
      v326 = v325;
      v327 = v326;
      v328 = v327;
      v329 = v328;
      v330 = v329;
      v331 = v330;
      v332 = v331;
      v333 = v332;
      v334 = v333;
      v335 = v334;
      v336 = v335;
      v337 = v336;
      v338 = v337;
      v339 = v338;
      v340 = v339;
      v341 = v340;
      v342 = v341;
      v343 = v342;
      v344 = v343;
      v345 = v344;
      v346 = v345;
      v347 = v346;
      v348 = v347;
      v349 = v348;
      v350 = v349;
      v351 = v350;
      v352 = v351;
      v353 = v352;
      v354 = v353;
      v355 = v354;
      v356 = v355;
      v357 = v356;
      v358 = v357;
      v359 = v358;
      v360 = v359;
      v361 = v360;
      v362 = v361;
      v363 = v362;
      v364 = v363;
      v365 = v364;
      v366 = v365;
      v367 = v366;
      v368 = v367;
      v369 = v368;
      v370 = v369;
      v371 = v370;
      v372 = v371;
      v373 = v372;
      v374 = v373;
      v375 = v374;
      v376 = v375;
      v377 = v376;
      v378 = v377;
      v379 = v378;
      v380 = v379;
      v381 = v380;
      v382 = v381;
      v383 = v382;
      v384 = v383;
      v385 = v384;
      v386 = v385;
      v387 = v386;
      v388 = v387;
      v389 = v388;
      v390 = v389;
      v391 = v390;
      v392 = v391;
      v393 = v392;
      v394 = v393;
      v395 = v394;
      v396 = v395;
      v397 = v396;
      v398 = v397;
      v399 = v398;
      v400 = v399;
      v401 = v400;
      v402 = v401;
      v403 = v402;
      v404 = v403;
      v405 = v404;
      v406 = v405;
      v407 = v406;
      v408 = v407;
      v409 = v408;
      v410 = v409;
      v411 = v410;
      v412 = v411;
      v413 = v412;
      v414 = v413;
      v415 = v414;
      v416 = v415;
      v417 = v416;
      v418 = v417;
      v419 = v418;
      v420 = v419;
      v421 = v420;
      v422 = v421;
      v423 = v422;
      v424 = v423;
      v425 = v424;
      v426 = v425;
      v427 = v426;
      v428 = v427;
      v429 = v428;
      v430 = v429;
      v431 = v430;
      v432 = v431;
      v433 = v432;
      v434 = v433;
      v435 = v434;
      v436 = v435;
      v437 = v436;
      v438 = v437;
      v439 = v438;
      v440 = v439;
      v441 = v440;
      v442 = v441;
      v443 = v442;
      v444 = v443;
      v445 = v444;
      v446 = v445;
      v447 = v446;
      v448 = v447;
      v449 = v448;
      v450 = v449;
      v451 = v450;
      v452 = v451;
      v453 = v452;
      v454 = v453;
      v455 = v454;
      v456 = v455;
      v457 = v456;
      v458 = v457;
      v459 = v458;
      v460 = v459;
      v461 = v460;
      v462 = v461;
      v463 = v462;
      v464 = v463;
      v465 = v464;
      v466 = v465;
      v467 = v466;
      v468 = v467;
      v469 = v468;
      v470 = v469;
      v471 = v470;
      v472 = v471;
      v473 = v472;
      v474 = v473;
      v475 = v474;
      v476 = v475;
      v477 = v476;
      v478 = v477;
      v479 = v478;
      v480 = v479;
      v481 = v480;
      v482 = v481;
      v483 = v482;
      v484 = v483;
      v485 = v484;
      v486 = v485;
      v487 = v486;
      v488 = v487;
      v489 = v488;
      v490 = v489;
      v491 = v490;
      v492 = v491;
      v493 = v492;
      v494 = v493;
      v495 = v494;
      v496 = v495;
      v497 = v496;
      v498 = v497;
      v499 = v498;
      v500 = v499; 
    }
   })";

  sp::SP::process(program, &pkb);

  // Time query
  auto start = std::chrono::high_resolution_clock::now();

  auto *result = pkb.getQueryFacade()->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  REQUIRE(result->size() == 500 * 500);
  std::cout << "Time taken by function: " << duration.count() << " microseconds"
            << std::endl;
}

TEST_CASE("Test NextStar worst case") {
  auto pkb = PKB();
  // 500 line simple program where each stmt affects the next stmt, within a
  // while loop
  const auto *program = R"(
  procedure main {
    while (x == y) {
      v1 = v500;
      v2 = v1;
      v3 = v2;
      v4 = v3;
      v5 = v4;
      v6 = v5;
      v7 = v6;
      v8 = v7;
      v9 = v8;
      v10 = v9;
      v11 = v10;
      v12 = v11;
      v13 = v12;
      v14 = v13;
      v15 = v14;
      v16 = v15;
      v17 = v16;
      v18 = v17;
      v19 = v18;
      v20 = v19;
      v21 = v20;
      v22 = v21;
      v23 = v22;
      v24 = v23;
      v25 = v24;
      v26 = v25;
      v27 = v26;
      v28 = v27;
      v29 = v28;
      v30 = v29;
      v31 = v30;
      v32 = v31;
      v33 = v32;
      v34 = v33;
      v35 = v34;
      v36 = v35;
      v37 = v36;
      v38 = v37;
      v39 = v38;
      v40 = v39;
      v41 = v40;
      v42 = v41;
      v43 = v42;
      v44 = v43;
      v45 = v44;
      v46 = v45;
      v47 = v46;
      v48 = v47;
      v49 = v48;
      v50 = v49;
      v51 = v50;
      v52 = v51;
      v53 = v52;
      v54 = v53;
      v55 = v54;
      v56 = v55;
      v57 = v56;
      v58 = v57;
      v59 = v58;
      v60 = v59;
      v61 = v60;
      v62 = v61;
      v63 = v62;
      v64 = v63;
      v65 = v64;
      v66 = v65;
      v67 = v66;
      v68 = v67;
      v69 = v68;
      v70 = v69;
      v71 = v70;
      v72 = v71;
      v73 = v72;
      v74 = v73;
      v75 = v74;
      v76 = v75;
      v77 = v76;
      v78 = v77;
      v79 = v78;
      v80 = v79;
      v81 = v80;
      v82 = v81;
      v83 = v82;
      v84 = v83;
      v85 = v84;
      v86 = v85;
      v87 = v86;
      v88 = v87;
      v89 = v88;
      v90 = v89;
      v91 = v90;
      v92 = v91;
      v93 = v92;
      v94 = v93;
      v95 = v94;
      v96 = v95;
      v97 = v96;
      v98 = v97;
      v99 = v98;
      v100 = v99;
      v101 = v100;
      v102 = v101;
      v103 = v102;
      v104 = v103;
      v105 = v104;
      v106 = v105;
      v107 = v106;
      v108 = v107;
      v109 = v108;
      v110 = v109;
      v111 = v110;
      v112 = v111;
      v113 = v112;
      v114 = v113;
      v115 = v114;
      v116 = v115;
      v117 = v116;
      v118 = v117;
      v119 = v118;
      v120 = v119;
      v121 = v120;
      v122 = v121;
      v123 = v122;
      v124 = v123;
      v125 = v124;
      v126 = v125;
      v127 = v126;
      v128 = v127;
      v129 = v128;
      v130 = v129;
      v131 = v130;
      v132 = v131;
      v133 = v132;
      v134 = v133;
      v135 = v134;
      v136 = v135;
      v137 = v136;
      v138 = v137;
      v139 = v138;
      v140 = v139;
      v141 = v140;
      v142 = v141;
      v143 = v142;
      v144 = v143;
      v145 = v144;
      v146 = v145;
      v147 = v146;
      v148 = v147;
      v149 = v148;
      v150 = v149;
      v151 = v150;
      v152 = v151;
      v153 = v152;
      v154 = v153;
      v155 = v154;
      v156 = v155;
      v157 = v156;
      v158 = v157;
      v159 = v158;
      v160 = v159;
      v161 = v160;
      v162 = v161;
      v163 = v162;
      v164 = v163;
      v165 = v164;
      v166 = v165;
      v167 = v166;
      v168 = v167;
      v169 = v168;
      v170 = v169;
      v171 = v170;
      v172 = v171;
      v173 = v172;
      v174 = v173;
      v175 = v174;
      v176 = v175;
      v177 = v176;
      v178 = v177;
      v179 = v178;
      v180 = v179;
      v181 = v180;
      v182 = v181;
      v183 = v182;
      v184 = v183;
      v185 = v184;
      v186 = v185;
      v187 = v186;
      v188 = v187;
      v189 = v188;
      v190 = v189;
      v191 = v190;
      v192 = v191;
      v193 = v192;
      v194 = v193;
      v195 = v194;
      v196 = v195;
      v197 = v196;
      v198 = v197;
      v199 = v198;
      v200 = v199;
      v201 = v200;
      v202 = v201;
      v203 = v202;
      v204 = v203;
      v205 = v204;
      v206 = v205;
      v207 = v206;
      v208 = v207;
      v209 = v208;
      v210 = v209;
      v211 = v210;
      v212 = v211;
      v213 = v212;
      v214 = v213;
      v215 = v214;
      v216 = v215;
      v217 = v216;
      v218 = v217;
      v219 = v218;
      v220 = v219;
      v221 = v220;
      v222 = v221;
      v223 = v222;
      v224 = v223;
      v225 = v224;
      v226 = v225;
      v227 = v226;
      v228 = v227;
      v229 = v228;
      v230 = v229;
      v231 = v230;
      v232 = v231;
      v233 = v232;
      v234 = v233;
      v235 = v234;
      v236 = v235;
      v237 = v236;
      v238 = v237;
      v239 = v238;
      v240 = v239;
      v241 = v240;
      v242 = v241;
      v243 = v242;
      v244 = v243;
      v245 = v244;
      v246 = v245;
      v247 = v246;
      v248 = v247;
      v249 = v248;
      v250 = v249;
      v251 = v250;
      v252 = v251;
      v253 = v252;
      v254 = v253;
      v255 = v254;
      v256 = v255;
      v257 = v256;
      v258 = v257;
      v259 = v258;
      v260 = v259;
      v261 = v260;
      v262 = v261;
      v263 = v262;
      v264 = v263;
      v265 = v264;
      v266 = v265;
      v267 = v266;
      v268 = v267;
      v269 = v268;
      v270 = v269;
      v271 = v270;
      v272 = v271;
      v273 = v272;
      v274 = v273;
      v275 = v274;
      v276 = v275;
      v277 = v276;
      v278 = v277;
      v279 = v278;
      v280 = v279;
      v281 = v280;
      v282 = v281;
      v283 = v282;
      v284 = v283;
      v285 = v284;
      v286 = v285;
      v287 = v286;
      v288 = v287;
      v289 = v288;
      v290 = v289;
      v291 = v290;
      v292 = v291;
      v293 = v292;
      v294 = v293;
      v295 = v294;
      v296 = v295;
      v297 = v296;
      v298 = v297;
      v299 = v298;
      v300 = v299;
      v301 = v300;
      v302 = v301;
      v303 = v302;
      v304 = v303;
      v305 = v304;
      v306 = v305;
      v307 = v306;
      v308 = v307;
      v309 = v308;
      v310 = v309;
      v311 = v310;
      v312 = v311;
      v313 = v312;
      v314 = v313;
      v315 = v314;
      v316 = v315;
      v317 = v316;
      v318 = v317;
      v319 = v318;
      v320 = v319;
      v321 = v320;
      v322 = v321;
      v323 = v322;
      v324 = v323;
      v325 = v324;
      v326 = v325;
      v327 = v326;
      v328 = v327;
      v329 = v328;
      v330 = v329;
      v331 = v330;
      v332 = v331;
      v333 = v332;
      v334 = v333;
      v335 = v334;
      v336 = v335;
      v337 = v336;
      v338 = v337;
      v339 = v338;
      v340 = v339;
      v341 = v340;
      v342 = v341;
      v343 = v342;
      v344 = v343;
      v345 = v344;
      v346 = v345;
      v347 = v346;
      v348 = v347;
      v349 = v348;
      v350 = v349;
      v351 = v350;
      v352 = v351;
      v353 = v352;
      v354 = v353;
      v355 = v354;
      v356 = v355;
      v357 = v356;
      v358 = v357;
      v359 = v358;
      v360 = v359;
      v361 = v360;
      v362 = v361;
      v363 = v362;
      v364 = v363;
      v365 = v364;
      v366 = v365;
      v367 = v366;
      v368 = v367;
      v369 = v368;
      v370 = v369;
      v371 = v370;
      v372 = v371;
      v373 = v372;
      v374 = v373;
      v375 = v374;
      v376 = v375;
      v377 = v376;
      v378 = v377;
      v379 = v378;
      v380 = v379;
      v381 = v380;
      v382 = v381;
      v383 = v382;
      v384 = v383;
      v385 = v384;
      v386 = v385;
      v387 = v386;
      v388 = v387;
      v389 = v388;
      v390 = v389;
      v391 = v390;
      v392 = v391;
      v393 = v392;
      v394 = v393;
      v395 = v394;
      v396 = v395;
      v397 = v396;
      v398 = v397;
      v399 = v398;
      v400 = v399;
      v401 = v400;
      v402 = v401;
      v403 = v402;
      v404 = v403;
      v405 = v404;
      v406 = v405;
      v407 = v406;
      v408 = v407;
      v409 = v408;
      v410 = v409;
      v411 = v410;
      v412 = v411;
      v413 = v412;
      v414 = v413;
      v415 = v414;
      v416 = v415;
      v417 = v416;
      v418 = v417;
      v419 = v418;
      v420 = v419;
      v421 = v420;
      v422 = v421;
      v423 = v422;
      v424 = v423;
      v425 = v424;
      v426 = v425;
      v427 = v426;
      v428 = v427;
      v429 = v428;
      v430 = v429;
      v431 = v430;
      v432 = v431;
      v433 = v432;
      v434 = v433;
      v435 = v434;
      v436 = v435;
      v437 = v436;
      v438 = v437;
      v439 = v438;
      v440 = v439;
      v441 = v440;
      v442 = v441;
      v443 = v442;
      v444 = v443;
      v445 = v444;
      v446 = v445;
      v447 = v446;
      v448 = v447;
      v449 = v448;
      v450 = v449;
      v451 = v450;
      v452 = v451;
      v453 = v452;
      v454 = v453;
      v455 = v454;
      v456 = v455;
      v457 = v456;
      v458 = v457;
      v459 = v458;
      v460 = v459;
      v461 = v460;
      v462 = v461;
      v463 = v462;
      v464 = v463;
      v465 = v464;
      v466 = v465;
      v467 = v466;
      v468 = v467;
      v469 = v468;
      v470 = v469;
      v471 = v470;
      v472 = v471;
      v473 = v472;
      v474 = v473;
      v475 = v474;
      v476 = v475;
      v477 = v476;
      v478 = v477;
      v479 = v478;
      v480 = v479;
      v481 = v480;
      v482 = v481;
      v483 = v482;
      v484 = v483;
      v485 = v484;
      v486 = v485;
      v487 = v486;
      v488 = v487;
      v489 = v488;
      v490 = v489;
      v491 = v490;
      v492 = v491;
      v493 = v492;
      v494 = v493;
      v495 = v494;
      v496 = v495;
      v497 = v496;
      v498 = v497;
      v499 = v498;
      v500 = v499;
    }
   })";

  sp::SP::process(program, &pkb);

  // Time query
  auto start = std::chrono::high_resolution_clock::now();

  auto *result = pkb.getQueryFacade()->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  REQUIRE(result->size() == 501 * 501);
  std::cout << "Time taken by function: " << duration.count() << " microseconds"
            << std::endl;
}

TEST_CASE("stress test unorderedmap with entitykey") {
  std::vector<std::shared_ptr<Entity>> entities;

  for (int i = 0; i < 1000000; i++) {
    auto entity = std::make_shared<AssignStatement>(i);

    entities.push_back(entity);
  }

  SECTION("actual test") {
    std::unordered_map<EntityKey, std::shared_ptr<Entity>> map;

    for (auto &entity : entities) {
      map.try_emplace(entity->getEntityKey(), entity);
    }
  }
}

// TEST_CASE("stress test  unorderedmap with pointer member") {
//   // make vector of entities
//   std::vector<AssignStatement> entities;
//
//   for (int i = 0; i < 1000000; i++) {
//     auto entity = AssignStatement(i);
//
//     entities.push_back(entity);
//   }
//
//   SECTION("actual test") {
//     std::unordered_map<EntityKey, Entity *> map;
//
//     for (auto &entity : entities) {
//       map.try_emplace(entity.getEntityKey(), &entity);
//     }
//   }
// }

// TEST_CASE("stress test unorderedset with int member") {
//   std::vector<AssignStatement> entities;
//
//   for (int i = 0; i < 1000000; i++) {
//     auto entity = AssignStatement(i);
//
//     entities.push_back(entity);
//   }
//
//   SECTION("actual test") {
//     std::unordered_map<int, Entity*> map;
//
//     for (auto &entity : entities) {
//         map.try_emplace(entity.getStatementNumber(), &entity);
//     }
//   }
//
// }

TEST_CASE("stress test unordered set for entity keys") {
  std::unordered_map<EntityKey, Entity &> map;
  for (int i = 0; i < 1000000; i++) {
    AssignStatement test = AssignStatement(i);
    //
    auto &key = test.getEntityKey();
    //     EntityKey key = EntityKey(&AssignStatement::getEntityTypeStatic(),
    //     i);

    map.try_emplace(key, test);
  }
}

TEST_CASE("stress test unordered map for int and entity") {
  std::unordered_map<int, Entity &> map;

  for (int i = 0; i < 1000000; i++) {
    AssignStatement test = AssignStatement(i);
    //
    //            EntityKey key = test->getEntityKey();
    //     EntityKey key = EntityKey(&AssignStatement::getEntityTypeStatic(),
    //     i);

    map.try_emplace(test.getStatementNumber(), test);
  }
}

TEST_CASE("stress test vector for int and entity") {
  std::vector<std::unique_ptr<Entity>> vec =
      std::vector<std::unique_ptr<Entity>>(1000000);

  for (int i = 0; i < 1000000; i++) {
    //
    //            EntityKey key = test->getEntityKey();
    //     EntityKey key = EntityKey(&AssignStatement::getEntityTypeStatic(),
    //     i);
    vec[i] = std::make_unique<AssignStatement>(i);
  }
}

// TEST_CASE("stress test unordered_map with int member") {
//   std::vector<AssignStatement> entities;
//
//   for (int i = 0; i < 1000000; i++) {
//     auto entity = AssignStatement(i);
//
//     entities.push_back(entity);
//   }
//
//   SECTION("actual test") {
//     std::unordered_map<int, Entity *> map;
//
//     for (auto &entity : entities) {
//       map.try_emplace(entity.getStatementNumber(), &entity);
//     }
//   }
// }

TEST_CASE("Stress test pass by value string") {
  std::vector<std::shared_ptr<Entity>> entities;

  for (int i = 0; i < 1000000; i++) {
    auto entity = std::make_shared<AssignStatement>(i);

    entities.push_back(entity);
  }

  SECTION("actual test") {
    std::unordered_map<EntityKey, std::shared_ptr<Entity>> map;

    for (auto &entity : entities) {
      map.try_emplace(entity->getEntityKey(), entity);
    }
  }
}

TEST_CASE("Test CFG relationship cache") {
  auto entityManager = std::make_shared<EntityManager>();
  auto relationshipManager =
      std::make_shared<RelationshipManager>(entityManager.get());
  auto patternManager = std::make_shared<PatternManager>();
  auto cfgManager = std::make_shared<CFGStorage>();
  auto populateFacade = std::make_shared<PopulateFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());
  auto queryFacade = std::make_shared<QueryFacade>(
      entityManager.get(), relationshipManager.get(), patternManager.get(),
      cfgManager.get());

  PKB pkb(entityManager, relationshipManager, patternManager, cfgManager);

  const auto *program = R"(
    procedure main {
          x = 0;
          i = 5;
          while (i != 0)  {
                x = x + 2*y;
                call Third;
                i = i - 1;
          }
          if (x==1) then {
               x = x+1;
          } else {
              z = 1;
          }
          z = z + x + i;
          y = z + 2;
          x = x * y + z;
          read x;
          y = x;
    }
    procedure Third {
        print x;
    })";

  sp::SP::process(program, &pkb);

  auto *pkb_querier = pkb.getQueryFacade();

  auto storageSnapshot = relationshipManager->getStoreAndCacheSizes();

  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 3,
              Statement::getEntityTypeStatic(), 4) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 8) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 9) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 7,
              Statement::getEntityTypeStatic(), 9) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 1,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 1,
              Statement::getEntityTypeStatic(), 9) != nullptr);
  auto storageSnapshot2 = relationshipManager->getStoreAndCacheSizes();

  // require first 4 entries of each snapshot to be the same as in snapshot 1

  queryFacade->clearCache();
  auto storageSnapshot3 = relationshipManager->getStoreAndCacheSizes();
  // Once cache clear, every entry to be the same as in snapshot 1
  for (int i = 0; i < 7; i++) {
    REQUIRE(storageSnapshot.at(i) == storageSnapshot3.at(i));
  }

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 8,
              Statement::getEntityTypeStatic(), 9) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              NextStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 6,
              Statement::getEntityTypeStatic(), 4) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 8) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 8,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 6) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 6) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 11) == nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 12) != nullptr);

  REQUIRE(pkb_querier->getRelationship(
              AffectsStarRelationship::getRelationshipTypeStatic(),
              Statement::getEntityTypeStatic(), 4,
              Statement::getEntityTypeStatic(), 14) == nullptr);

  auto storageSnapshot4 = relationshipManager->getStoreAndCacheSizes();

  // for (int i = 0; i < 4; i++) {
  //   REQUIRE(storageSnapshot.at(i) == storageSnapshot4.at(i));
  // }

  queryFacade->clearCache();
  auto storageSnapshot5 = relationshipManager->getStoreAndCacheSizes();
  for (int i = 0; i < 7; i++) {
    REQUIRE(storageSnapshot.at(i) == storageSnapshot5.at(i));
  }

  auto *temp = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  REQUIRE(temp->size() == 100);

  auto *temp2 = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp2->size() == 47);

  auto *temp3 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), AssignStatement::getEntityTypeStatic());

  REQUIRE(temp3->size() == 27);

  auto storageSnapshot6 = relationshipManager->getStoreAndCacheSizes();
  // for (int i = 0; i < 4; i++) {
  //   REQUIRE(storageSnapshot.at(i) == storageSnapshot6.at(i));
  // }
  queryFacade->clearCache();
  auto storageSnapshot7 = relationshipManager->getStoreAndCacheSizes();
  for (int i = 0; i < 7; i++) {
    REQUIRE(storageSnapshot.at(i) == storageSnapshot7.at(i));
  }

  auto *temp4 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      ReadStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp4->empty());

  auto *temp5 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp5->size() == 27);

  auto pairVector = std::vector<std::pair<std::string, std::string>>();

  for (auto *relationship : *temp5) {
    auto val1 = *relationship->getLeftHandEntity()->getEntityValue();
    auto val2 = *relationship->getRightHandEntity()->getEntityValue();
    pairVector.emplace_back(val1, val2);
  }

  REQUIRE(pairVector.size() == 27);

  auto expectedPairs = std::vector<std::pair<std::string, std::string>>(
      {{"11", "12"}, {"9", "10"}, {"9", "11"}, {"9", "12"}, {"6", "10"}, {"6", "11"}, {"6", "12"}, {"6", "6"}, {"10", "11"}, {"10", "12"}, {"8", "10"}, {"8", "11"}, {"8", "12"}, {"4", "10"}, {"4", "11"}, {"4", "12"}, {"4", "8"}, {"4", "4"}, {"2", "10"}, {"2", "11"}, {"2", "12"}, {"2", "6"}, {"1", "10"}, {"1", "11"}, {"1", "12"}, {"1", "8"}, {"1", "4"}});

  for (const auto &pair : expectedPairs) {
    REQUIRE(std::find(pairVector.begin(), pairVector.end(), pair) !=
            pairVector.end());
  }

  auto *temp6 = pkb_querier->getRelationshipsByTypes(
      AffectsStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), Statement::getEntityTypeStatic());

  REQUIRE(temp6->size() == 27);

  auto *temp7 = pkb_querier->getRelationshipsByTypes(
      AffectsRelationship::getRelationshipTypeStatic(),
      AssignStatement::getEntityTypeStatic(),
      AssignStatement::getEntityTypeStatic());

  REQUIRE(temp7->size() == 18);
  auto *temp8 =
          pkb_querier->getRightEntitiesRelatedToGivenLeftEntity(
                  AffectsStarRelationship::getRelationshipTypeStatic(),
                  Statement::getEntityTypeStatic(), 1,
                  Statement::getEntityTypeStatic());

  REQUIRE(temp8->size() == 5);

  // auto storageSnapshot8 = relationshipManager->getStoreAndCacheSizes();
  // for (int i = 0; i < 4; i++) {
  //   REQUIRE(storageSnapshot.at(i) == storageSnapshot8.at(i));
  // }
  queryFacade->clearCache();
  auto storageSnapshot9 = relationshipManager->getStoreAndCacheSizes();
  for (int i = 0; i < 7; i++) {
    REQUIRE(storageSnapshot.at(i) == storageSnapshot9.at(i));
  }

  auto *temp9 =
          pkb_querier->getLeftEntitiesRelatedToGivenRightEntity(
                  NextStarRelationship::getRelationshipTypeStatic(),
                  CallStatement::getEntityTypeStatic(),
                  Statement::getEntityTypeStatic(), 8);

  REQUIRE(temp9->size() == 1);
  REQUIRE(*temp9->at(0)->getEntityValue() == "5");

  auto *temp10 = pkb_querier->getRelationshipsByTypes(
      NextStarRelationship::getRelationshipTypeStatic(),
      CallStatement::getEntityTypeStatic(), Statement::getEntityTypeStatic());
  REQUIRE(temp10->size() == 12);

  auto *temp11 =
          pkb_querier->getLeftEntitiesRelatedToGivenRightEntity(
                  NextStarRelationship::getRelationshipTypeStatic(),
                  CallStatement::getEntityTypeStatic(),
                  Statement::getEntityTypeStatic(), 8);

  auto *temp12 = pkb_querier->getRelationship(
      NextStarRelationship::getRelationshipTypeStatic(),
      CallStatement::getEntityTypeStatic(), 5, Statement::getEntityTypeStatic(),
      8);

  auto *temp13 = pkb_querier->getRelationship(
      NextStarRelationship::getRelationshipTypeStatic(),
      Statement::getEntityTypeStatic(), 5, Statement::getEntityTypeStatic(), 8);

  REQUIRE(*temp11->at(0)->getEntityValue() == "5");
  REQUIRE(*temp12->getLeftHandEntity()->getEntityValue() == "5");
  REQUIRE(*temp13->getLeftHandEntity()->getEntityValue() == "5");

  // auto storageSnapshot10 = relationshipManager->getStoreAndCacheSizes();
  // for (int i = 0; i < 4; i++) {
  //   REQUIRE(storageSnapshot.at(i) == storageSnapshot10.at(i));
  // }
  queryFacade->clearCache();
  auto storageSnapshot11 = relationshipManager->getStoreAndCacheSizes();
  for (int i = 0; i < 7; i++) {
    REQUIRE(storageSnapshot.at(i) == storageSnapshot11.at(i));
  }
}

TEST_CASE("Test get all whiles") {
  auto pkb = PKB();

  const auto *program = R"(
 procedure main {
  read x;
  read y;
  print x;
  z = x + y + 1;
  x = z + 1;
  y = x + 1;

  while (x < 10) {
    print x;
    x = x + 1;
  }

  if (x < 10) then {
    print x;
    while (1 == 1) {
      read x;
      print x;
    }
    a = 1;
  } else {
    print y;
  }
  call funcY;
}

procedure funcY {
  print y;

  a = x + 1;
  b = a + 1;

  if (a * b < x / 5) then {
    print a;
    read x;
    read y;
  } else {
    print b;
  }
  read x;
}
)";

  sp::SP::process(program, &pkb);
  auto eval = qps_test::RunQuery(
      "while w; Select <w> such that Follows*(13, _)", *pkb.getQueryFacade());
  REQUIRE(eval == std::unordered_set<std::string>{"7", "12"});
}

TEST_CASE("Test reverse affects evaluation") {
  auto pkb = PKB();

  const auto *program = R"(
procedure hello {
            read a;
            read b;
            read c;
            read d;
    }
    procedure main {
            a = x + y;
            b = a + y;
            c = 1;
            while (x == 1) {
                z = b + a;
                x = a;
                if (x == 2) then {
                            c = d + a;
                    } else {
                    d = d + a;
                    while (x == 1) {
                        print x;
                        print y;
                        while (y == 2) {
                            c = a + x;
                            if (x == 2) then {
                                        d = c;
                                } else {
                                a = 2 * a + x;
                                b = 2 * b;
                            }
                        }
                    }
                    c = 2 * a;
                    a = b + a;
                    x = a;
                }
                call hello;
                y = z + x + a;
            }
            c = a;
            a = 2 * a;
            d = b;
    }
)";

  sp::SP::process(program, &pkb);
  auto eval = qps_test::RunQuery(
      "assign a; stmt s;\n"
      "Select s such that Affects(s,27)",
      *pkb.getQueryFacade());
  REQUIRE(eval == std::unordered_set<std::string>{"9", "10", "25"});
}

}  // namespace qps

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
