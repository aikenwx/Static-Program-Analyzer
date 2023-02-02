
#include "PrintStatement.h"
#include "WhileStatement.h"
#include "IfStatement.h"
#include "CallStatement.h"
#include "AssignStatement.h"
#include "ReadStatement.h"
#include "Entity.h"

#include "catch.hpp"
using namespace std;

TEST_CASE("Entities instantiate") {
    PrintStatement* printStatement = new PrintStatement(2);
    ReadStatement* readStatement = new ReadStatement(3);
    AssignStatement* assignStatement = new AssignStatement(4);
    CallStatement* callStatement = new CallStatement(5);
    WhileStatement* whileStatement = new WhileStatement(6);
    IfStatement* ifStatement = new IfStatement(7);

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
}

TEST_CASE("Entities retrieve their stored values") {
    PrintStatement* printStatement = new PrintStatement(2);
    ReadStatement* readStatement = new ReadStatement(3);
    AssignStatement* assignStatement = new AssignStatement(4);
    CallStatement* callStatement = new CallStatement(5);
    WhileStatement* whileStatement = new WhileStatement(6);
    IfStatement* ifStatement = new IfStatement(7);

    REQUIRE(printStatement->getEntityValue() == "2");
    REQUIRE(readStatement->getEntityValue() == "3");
    REQUIRE(assignStatement->getEntityValue() == "4");
    REQUIRE(callStatement->getEntityValue() == "5");
    REQUIRE(whileStatement->getEntityValue() == "6");
    REQUIRE(ifStatement->getEntityValue() == "7");


    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
}

TEST_CASE("Entities can be compared") {
    PrintStatement* printStatement = new PrintStatement(2);
    PrintStatement* printStatement2 = new PrintStatement(2);
    PrintStatement* printStatement3 = new PrintStatement(3);

    ReadStatement* readStatement = new ReadStatement(3);
    ReadStatement* readStatement2 = new ReadStatement(3);
    ReadStatement* readStatement3 = new ReadStatement(4);

    AssignStatement* assignStatement = new AssignStatement(4);
    AssignStatement* assignStatement2 = new AssignStatement(4);
    AssignStatement* assignStatement3 = new AssignStatement(5);

    CallStatement* callStatement = new CallStatement(5);
    CallStatement* callStatement2 = new CallStatement(5);
    CallStatement* callStatement3 = new CallStatement(6);

    WhileStatement* whileStatement = new WhileStatement(6);
    WhileStatement* whileStatement2 = new WhileStatement(6);
    WhileStatement* whileStatement3 = new WhileStatement(7);

    IfStatement* ifStatement = new IfStatement(7);
    IfStatement* ifStatement2 = new IfStatement(7);
    IfStatement* ifStatement3 = new IfStatement(8);

    REQUIRE(printStatement->equals(printStatement));
    REQUIRE(readStatement->equals(readStatement));
    REQUIRE(assignStatement->equals(assignStatement));
    REQUIRE(callStatement->equals(callStatement));
    REQUIRE(whileStatement->equals(whileStatement));
    REQUIRE(ifStatement->equals(ifStatement));

    REQUIRE(printStatement->equals(printStatement2));
    REQUIRE(readStatement->equals(readStatement2));
    REQUIRE(assignStatement->equals(assignStatement2));
    REQUIRE(callStatement->equals(callStatement2));
    REQUIRE(whileStatement->equals(whileStatement2));
    REQUIRE(ifStatement->equals(ifStatement2));

    REQUIRE(!printStatement->equals(printStatement3));
    REQUIRE(!readStatement->equals(readStatement3));
    REQUIRE(!assignStatement->equals(assignStatement3));
    REQUIRE(!callStatement->equals(callStatement3));
    REQUIRE(!whileStatement->equals(whileStatement3));
    REQUIRE(!ifStatement->equals(ifStatement3));

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
}