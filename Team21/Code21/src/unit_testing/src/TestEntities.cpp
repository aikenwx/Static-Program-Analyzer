// add guards


#include "PKBStorageClasses/EntityClasses/PrintStatement.h"
#include "PKBStorageClasses/EntityClasses/WhileStatement.h"
#include "PKBStorageClasses/EntityClasses/IfStatement.h"
#include "PKBStorageClasses/EntityClasses/CallStatement.h"
#include "PKBStorageClasses/EntityClasses/AssignStatement.h"
#include "PKBStorageClasses/EntityClasses/ReadStatement.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/EntityClasses/Procedure.h"
#include "PKBStorageClasses/EntityClasses/Variable.h"
#include "PKBStorageClasses/EntityClasses/Constant.h"


#include "catch.hpp"

using namespace std;

TEST_CASE("Entities can instantiate") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    AssignStatement *assignStatement = new AssignStatement(4);
    CallStatement *callStatement = new CallStatement(5);
    WhileStatement *whileStatement = new WhileStatement(6);
    IfStatement *ifStatement = new IfStatement(7);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");
    Constant *constant = new Constant(1);


    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
    delete procedure;
    delete variable;
    delete constant;
}

TEST_CASE("Entities retrieve their stored values") {
    PrintStatement *printStatement = new PrintStatement(2);
    ReadStatement *readStatement = new ReadStatement(3);
    AssignStatement *assignStatement = new AssignStatement(4);
    CallStatement *callStatement = new CallStatement(5);
    WhileStatement *whileStatement = new WhileStatement(6);
    IfStatement *ifStatement = new IfStatement(7);
    Procedure *procedure = new Procedure("procedure");
    Variable *variable = new Variable("variable");
    Constant *constant = new Constant(1);


    REQUIRE(printStatement->getEntityValue() == "2");
    REQUIRE(readStatement->getEntityValue() == "3");
    REQUIRE(assignStatement->getEntityValue() == "4");
    REQUIRE(callStatement->getEntityValue() == "5");
    REQUIRE(whileStatement->getEntityValue() == "6");
    REQUIRE(ifStatement->getEntityValue() == "7");
    REQUIRE(procedure->getEntityValue() == "procedure");
    REQUIRE(variable->getEntityValue() == "variable");
    REQUIRE(constant->getEntityValue() == "1");


    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
    delete procedure;
    delete variable;
    delete constant;
}

TEST_CASE("Entities can be compared") {
    PrintStatement *printStatement = new PrintStatement(2);
    PrintStatement *printStatement2 = new PrintStatement(2);
    PrintStatement *printStatement3 = new PrintStatement(3);

    ReadStatement *readStatement = new ReadStatement(3);
    ReadStatement *readStatement2 = new ReadStatement(3);
    ReadStatement *readStatement3 = new ReadStatement(4);

    AssignStatement *assignStatement = new AssignStatement(4);
    AssignStatement *assignStatement2 = new AssignStatement(4);
    AssignStatement *assignStatement3 = new AssignStatement(5);

    CallStatement *callStatement = new CallStatement(5);
    CallStatement *callStatement2 = new CallStatement(5);
    CallStatement *callStatement3 = new CallStatement(6);

    WhileStatement *whileStatement = new WhileStatement(6);
    WhileStatement *whileStatement2 = new WhileStatement(6);
    WhileStatement *whileStatement3 = new WhileStatement(7);

    IfStatement *ifStatement = new IfStatement(7);
    IfStatement *ifStatement2 = new IfStatement(7);
    IfStatement *ifStatement3 = new IfStatement(8);

    Variable *variable = new Variable("variable");
    Variable *variable2 = new Variable("variable");
    Variable *variable3 = new Variable("variable2");

    Procedure *procedure = new Procedure("procedure");
    Procedure *procedure2 = new Procedure("procedure");
    Procedure *procedure3 = new Procedure("procedure2");

    Constant *constant = new Constant(1);
    Constant *constant2 = new Constant(1);
    Constant *constant3 = new Constant(2);

    REQUIRE(printStatement->equals(printStatement));
    REQUIRE(readStatement->equals(readStatement));
    REQUIRE(assignStatement->equals(assignStatement));
    REQUIRE(callStatement->equals(callStatement));
    REQUIRE(whileStatement->equals(whileStatement));
    REQUIRE(ifStatement->equals(ifStatement));
    REQUIRE(procedure->equals(procedure));
    REQUIRE(variable->equals(variable));
    REQUIRE(constant->equals(constant));

    REQUIRE(printStatement->equals(printStatement2));
    REQUIRE(readStatement->equals(readStatement2));
    REQUIRE(assignStatement->equals(assignStatement2));
    REQUIRE(callStatement->equals(callStatement2));
    REQUIRE(whileStatement->equals(whileStatement2));
    REQUIRE(ifStatement->equals(ifStatement2));
    REQUIRE(procedure->equals(procedure2));
    REQUIRE(variable->equals(variable2));
    REQUIRE(constant->equals(constant2));

    REQUIRE(!printStatement->equals(printStatement3));
    REQUIRE(!readStatement->equals(readStatement3));
    REQUIRE(!assignStatement->equals(assignStatement3));
    REQUIRE(!callStatement->equals(callStatement3));
    REQUIRE(!whileStatement->equals(whileStatement3));
    REQUIRE(!ifStatement->equals(ifStatement3));
    REQUIRE(!procedure->equals(procedure3));
    REQUIRE(!variable->equals(variable3));
    REQUIRE(!constant->equals(constant3));

    REQUIRE(!printStatement->equals(readStatement));
    REQUIRE(!assignStatement->equals(callStatement));
    REQUIRE(!callStatement->equals(whileStatement));
    REQUIRE(!whileStatement->equals(ifStatement));
    REQUIRE(!ifStatement->equals(printStatement));
    REQUIRE(!printStatement->equals(variable));
    REQUIRE(!variable->equals(constant));
    REQUIRE(!constant->equals(procedure));
    REQUIRE(!procedure->equals(assignStatement));

    delete printStatement;
    delete readStatement;
    delete assignStatement;
    delete callStatement;
    delete whileStatement;
    delete ifStatement;
    delete procedure;
    delete variable;
    delete constant;
}
