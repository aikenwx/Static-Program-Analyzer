//
// Created by Aiken Wong on 2/2/23.
//
#include "Statement.h"

std::string * Statement::getEntityValue() {
    return this->statementNumberString.get();
};

const int Statement::getStatementNumber() {
    return this->statementNumber;
}
