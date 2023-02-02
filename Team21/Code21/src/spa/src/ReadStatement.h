#ifndef SPA_READSTATEMENT_H
#define SPA_READSTATEMENT_H

#include "Statement.h"

class ReadStatement : public Statement {
private:
    int statementNumber;
public:
    bool equals(Entity *otherEntity) override;

    ReadStatement(int statementNumber);
};


#endif
