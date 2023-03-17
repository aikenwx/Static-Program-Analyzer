// add guards
#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBtestHelpers.h"

TEST_CASE("Entities can instantiate") {
  auto printStatement = std::make_shared<PrintStatement>(ONE);
  auto readStatement = std::make_shared<ReadStatement>(TWO);
  auto assignStatement = std::make_shared<AssignStatement>(THREE);
  auto callStatement = std::make_shared<CallStatement>(FOUR);
  auto whileStatement = std::make_shared<WhileStatement>(FIVE);
  auto ifStatement = std::make_shared<IfStatement>(SIX);
  auto procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variable =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto constant = std::make_shared<Constant>(ONE);
}

TEST_CASE("Entities retrieve their stored values") {
  auto printStatement = std::make_shared<PrintStatement>(ONE);
  auto readStatement = std::make_shared<ReadStatement>(TWO);
  auto assignStatement = std::make_shared<AssignStatement>(THREE);
  auto callStatement = std::make_shared<CallStatement>(FOUR);
  auto whileStatement = std::make_shared<WhileStatement>(FIVE);
  auto ifStatement = std::make_shared<IfStatement>(SIX);
  auto procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variable =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto constant = std::make_shared<Constant>(ONE);

  REQUIRE(*printStatement->getEntityValue() == "1");
  REQUIRE(*readStatement->getEntityValue() == "2");

  REQUIRE(*assignStatement->getEntityValue() == "3");
  REQUIRE(*callStatement->getEntityValue() == "4");
  REQUIRE(*whileStatement->getEntityValue() == "5");
  REQUIRE(*ifStatement->getEntityValue() == "6");
  REQUIRE(*procedure->getEntityValue() == "procedure");
  REQUIRE(*variable->getEntityValue() == "variable");
  REQUIRE(*constant->getEntityValue() == "1");
}

TEST_CASE("Entities can be compared") {
  auto printStatement = std::make_shared<PrintStatement>(TWO);
  auto printStatement2 = std::make_shared<PrintStatement>(TWO);
  auto printStatement3 = std::make_shared<PrintStatement>(THREE);

  auto readStatement = std::make_shared<ReadStatement>(THREE);
  auto readStatement2 = std::make_shared<ReadStatement>(THREE);
  auto readStatement3 = std::make_shared<ReadStatement>(FOUR);

  auto assignStatement = std::make_shared<AssignStatement>(FOUR);
  auto assignStatement2 = std::make_shared<AssignStatement>(FOUR);
  auto assignStatement3 = std::make_shared<AssignStatement>(FIVE);

  auto callStatement = std::make_shared<CallStatement>(FIVE);
  auto callStatement2 = std::make_shared<CallStatement>(FIVE);
  auto callStatement3 = std::make_shared<CallStatement>(SIX);

  auto whileStatement = std::make_shared<WhileStatement>(SIX);
  auto whileStatement2 = std::make_shared<WhileStatement>(SIX);
  auto whileStatement3 = std::make_shared<WhileStatement>(SEVEN);

  auto ifStatement = std::make_shared<IfStatement>(SEVEN);
  auto ifStatement2 = std::make_shared<IfStatement>(SEVEN);
  auto ifStatement3 = std::make_shared<IfStatement>(EIGHT);

  auto variable =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto variable2 =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto variable3 =
      std::make_shared<Variable>(std::make_shared<std::string>("variable2"));

  auto procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto procedure2 =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto procedure3 =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure2"));

  auto constant = std::make_shared<Constant>(ONE);
  auto constant2 = std::make_shared<Constant>(ONE);
  auto constant3 = std::make_shared<Constant>(TWO);

  REQUIRE(printStatement->equals(printStatement.get()));
  REQUIRE(readStatement->equals(readStatement.get()));
  REQUIRE(assignStatement->equals(assignStatement.get()));
  REQUIRE(callStatement->equals(callStatement.get()));
  REQUIRE(whileStatement->equals(whileStatement.get()));
  REQUIRE(ifStatement->equals(ifStatement.get()));
  REQUIRE(procedure->equals(procedure.get()));
  REQUIRE(variable->equals(variable.get()));
  REQUIRE(constant->equals(constant.get()));
  printStatement->equals(printStatement2.get());
  REQUIRE(printStatement->equals(printStatement2.get()));
  REQUIRE(readStatement->equals(readStatement2.get()));
  REQUIRE(assignStatement->equals(assignStatement2.get()));
  REQUIRE(callStatement->equals(callStatement2.get()));
  REQUIRE(whileStatement->equals(whileStatement2.get()));
  REQUIRE(ifStatement->equals(ifStatement2.get()));
  REQUIRE(procedure->equals(procedure2.get()));
  REQUIRE(variable->equals(variable2.get()));
  REQUIRE(constant->equals(constant2.get()));

  REQUIRE(!printStatement->equals(printStatement3.get()));
  REQUIRE(!readStatement->equals(readStatement3.get()));
  REQUIRE(!assignStatement->equals(assignStatement3.get()));
  REQUIRE(!callStatement->equals(callStatement3.get()));
  REQUIRE(!whileStatement->equals(whileStatement3.get()));
  REQUIRE(!ifStatement->equals(ifStatement3.get()));
  REQUIRE(!procedure->equals(procedure3.get()));
  REQUIRE(!variable->equals(variable3.get()));
  REQUIRE(!constant->equals(constant3.get()));

  REQUIRE(!printStatement->equals(readStatement.get()));
  REQUIRE(!assignStatement->equals(callStatement.get()));
  REQUIRE(!callStatement->equals(whileStatement.get()));
  REQUIRE(!whileStatement->equals(ifStatement.get()));
  REQUIRE(!ifStatement->equals(printStatement.get()));
  REQUIRE(!printStatement->equals(variable.get()));
  REQUIRE(!variable->equals(constant.get()));
  REQUIRE(!constant->equals(procedure.get()));
  REQUIRE(!procedure->equals(assignStatement.get()));
}

TEST_CASE("Assign Statement stores post-fix expression") {
  auto assignStatement = std::make_shared<AssignStatement>(THREE);
  assignStatement->setPostfixExpression(std::make_shared<std::string>("xy+"));
  REQUIRE(*assignStatement->getPostFixExpression() == "xy+");
}

TEST_CASE(
    "Assign Statement throws error if getting post-fix expression before "
    "setting") {
  auto assignStatement = std::make_shared<AssignStatement>(THREE);
  REQUIRE_THROWS(assignStatement->getPostFixExpression());
}
