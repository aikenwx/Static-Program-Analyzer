#include <utility>
#include <iostream>

#include "catch.hpp"

#include "query_evaluators/QPS.h"
#include "sp/sp.h"

TEST_CASE("System Test") {
  PKB *pkb = new PKB();
  sp::SP sp = sp::SP();
  std::string program = R"(procedure followsPat {
 x = x + 1000 - z + y;
 read y;
 while ( i != 0) {
  z = x + z - 10;
  if (j > 5) then {
  	y = x + 20 + z;
        read a;
        k = k + x - y;
  } else {
  	z = 100 + y - z;
  }
  print y;
 }
 read x;
})";
  sp.process(program, pkb);
//  auto ans = pkb->getQueryFacade()->getFollowsRelationshipsByLeftAndRightEntityTypes(EntityType::STATEMENT,
//                                                                                     EntityType::STATEMENT);
//  for (auto r : *ans) {
//    std::cout << *r->getLeftHandEntity()->getEntityValue() << " " << *r->getRightHandEntity()->getEntityValue() << "\n";
//  }
  std::list<std::string> results;
  std::string query = "assign a; stmt s; Select a such that Follows(_, a) pattern a(\"z\",_)";
  qps::QPS::evaluate(query, results, *pkb->getQueryFacade());
  delete pkb;
//  auto ans =
//      pkb.getQueryFacade()->getUsesRelationshipsByLeftAndRightEntityTypes(EntityType::STATEMENT, EntityType::VARIABLE);
//  for (auto r : *ans) {
//    std::cout << *r->getLeftHandEntity()->getEntityValue() << " " << *r->getRightHandEntity()->getEntityValue() << "\n";
//  }
}
