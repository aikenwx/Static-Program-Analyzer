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

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

TEST_CASE("Entities can instantiate") {
  auto printStatement = std::make_shared<PrintStatement>(1);
  auto readStatement = std::make_shared<ReadStatement>(2);
  auto assignStatement = std::make_shared<AssignStatement>(3);
  auto callStatement = std::make_shared<CallStatement>(4);
  auto whileStatement = std::make_shared<WhileStatement>(5);
  auto ifStatement = std::make_shared<IfStatement>(6);
  auto procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variable =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto constant = std::make_shared<Constant>(1);
}

TEST_CASE("Entities retrieve their stored values") {
  auto printStatement = std::make_shared<PrintStatement>(1);
  auto readStatement = std::make_shared<ReadStatement>(2);
  auto assignStatement = std::make_shared<AssignStatement>(3);
  auto callStatement = std::make_shared<CallStatement>(4);
  auto whileStatement = std::make_shared<WhileStatement>(5);
  auto ifStatement = std::make_shared<IfStatement>(6);
  auto procedure =
      std::make_shared<Procedure>(std::make_shared<std::string>("procedure"));
  auto variable =
      std::make_shared<Variable>(std::make_shared<std::string>("variable"));
  auto constant = std::make_shared<Constant>(1);

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
  auto printStatement = std::make_shared<PrintStatement>(2);
  auto printStatement2 = std::make_shared<PrintStatement>(2);
  auto printStatement3 = std::make_shared<PrintStatement>(3);

  auto readStatement = std::make_shared<ReadStatement>(3);
  auto readStatement2 = std::make_shared<ReadStatement>(3);
  auto readStatement3 = std::make_shared<ReadStatement>(4);

  auto assignStatement = std::make_shared<AssignStatement>(4);
  auto assignStatement2 = std::make_shared<AssignStatement>(4);
  auto assignStatement3 = std::make_shared<AssignStatement>(5);

  auto callStatement = std::make_shared<CallStatement>(5);
  auto callStatement2 = std::make_shared<CallStatement>(5);
  auto callStatement3 = std::make_shared<CallStatement>(6);

  auto whileStatement = std::make_shared<WhileStatement>(6);
  auto whileStatement2 = std::make_shared<WhileStatement>(6);
  auto whileStatement3 = std::make_shared<WhileStatement>(7);

  auto ifStatement = std::make_shared<IfStatement>(7);
  auto ifStatement2 = std::make_shared<IfStatement>(7);
  auto ifStatement3 = std::make_shared<IfStatement>(8);

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

  auto constant = std::make_shared<Constant>(1);
  auto constant2 = std::make_shared<Constant>(1);
  auto constant3 = std::make_shared<Constant>(2);

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
  auto assignStatement = std::make_shared<AssignStatement>(3);
  assignStatement->setPostfixExpression(std::make_shared<std::string>("xy+"));
  REQUIRE(*assignStatement->getPostFixExpression() == "xy+");
}

TEST_CASE(
    "Assign Statement throws error if getting post-fix expression before "
    "setting") {
  auto assignStatement = std::make_shared<AssignStatement>(3);
  REQUIRE_THROWS(assignStatement->getPostFixExpression());
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
