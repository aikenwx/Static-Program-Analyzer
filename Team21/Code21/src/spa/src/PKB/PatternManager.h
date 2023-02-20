//
// Created by Aiken Wong on 12/2/23.
//

#ifndef SPA_PATTERNMANAGER_H
#define SPA_PATTERNMANAGER_H

#include <string>
#include "../PKBStorageClasses/EntityClasses/AssignStatement.h"

class PatternManager {
public:
    PatternManager() = default;
    void storeAssignStatementPostfixExpression(AssignStatement* assignStatement, std::string* postfixExpression);
};


#endif //SPA_PATTERNMANAGER_H
