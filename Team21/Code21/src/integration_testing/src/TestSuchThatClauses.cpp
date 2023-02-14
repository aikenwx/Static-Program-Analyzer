////Sample program
////procedure computeCentroid {
////    01      count = 0;
////    02      cenX = 0;
////    03      cenY = 0;
////    04      call readPoint;
////    05      while ((x != 0) && (y != 0)) {
////      06          count = count + 1;
////      07          cenX = cenX + x;
////      08          cenY = cenY + y;
////      09          call readPoint;
////    }
////    10      if (count == 0) then {
////      11          flag = 1;
////    } else {
////      12          cenX = cenX / count;
////      13          cenY = cenY / count;
////    }
////    14      normSq = cenX * cenX + cenY * cenY;
////}
//TEST_CASE("QPS can work with select and such that parent clause") {
//Data data;
//data[qps::DesignEntity::VARIABLE] = {"count", "cenX", "cenY", "flag", "cenX", "cenY", "x", "y"};
//data[qps::DesignEntity::CONSTANT] = {"0", "1"};
//data[qps::DesignEntity::ASSIGN] = {"1", "2", "3", "6", "7", "8", "11", "12", "13", "14"};
//data[qps::DesignEntity::READ] = {};
//data[qps::DesignEntity::CALL] = {"4", "9"};
//data[qps::DesignEntity::IF] = {"10"};
//data[qps::DesignEntity::WHILE] = {"5"};
//data[qps::DesignEntity::PRINT] = {};
//data[qps::DesignEntity::PROCEDURE] = {"computeCentroid", "readPoint"};
//
//PopulatePKBHelper pkb_helper;
//PopulateEntities(pkb_helper, data);
//QueryFacade *pkb_querier = pkb_helper.GetQuerier();
//PopulateFacade *pkb_populater = pkb_helper.GetPopulater();
//pkb_populater->storeParentRelationship(5, 6);
//pkb_populater->storeParentRelationship(5, 7);
//pkb_populater->storeParentRelationship(5, 8);
//pkb_populater->storeParentRelationship(5, 9);
//
//pkb_populater->storeParentRelationship(10, 11);
//pkb_populater->storeParentRelationship(10, 12);
//pkb_populater->storeParentRelationship(10, 13);
//
//pkb_populater->storeStatementModifiesVariableRelationship(6, "count");
//pkb_populater->storeStatementModifiesVariableRelationship(7, "cenX");
//pkb_populater->storeStatementModifiesVariableRelationship(7, "cenY");
//pkb_populater->storeStatementModifiesVariableRelationship(12, "cenX");
//pkb_populater->storeStatementModifiesVariableRelationship(13, "cenY");
//
//
//
//
//SECTION("QPS can retrieve all variables") {
//REQUIRE(RunQuery("variable h; Select h", *pkb_querier) == data[qps::DesignEntity::VARIABLE]);
//}
//
//SECTION("QPS can retrieve variables modified in a single staement") {
//std::unordered_set<std::string> expected{"count"};
//REQUIRE(RunQuery("variable v; Select v such that Modifies(6, v)", *pkb_querier) == expected);
//}
//
//SECTION("QPS can retrieve all variables modified in a assign a statement") {
//std::unordered_set<std::string> expected{"count", "cenX", "cenY"};
//REQUIRE(RunQuery("variable v; assign a; Select v such that Modifies(a, v)", *pkb_querier) == expected);
//}
//
//SECTION("QPS can retrieve statements that are parent of a statement") {
//std::unordered_set<std::string> expected{"5"};
//REQUIRE(RunQuery("stmt s; Select s such that Parent(s, 7)", *pkb_querier) == expected);
//}
//
//SECTION("QPS can retrieve all statements that are a child") {
//std::unordered_set<std::string> expected{"6", "7", "8", "9", "11", "12", "13"};
//REQUIRE(RunQuery("stmt s; Select s such that Parent(_, s)", *pkb_querier) == expected);
//}
//
//SECTION("QPS can retrieve all assigments that are a child") {
//std::unordered_set<std::string> expected{"6", "7", "8", "11", "12", "13"};
//REQUIRE(RunQuery("assign a; Select a such that Parent(_, a)", *pkb_querier) == expected);
//}
//
//SECTION("QPS can retrieve all calls that are a child") {
//std::unordered_set<std::string> expected{"9"};
//REQUIRE(RunQuery("call c; Select c such that Parent(_, c)", *pkb_querier) == expected);
//}
//}
