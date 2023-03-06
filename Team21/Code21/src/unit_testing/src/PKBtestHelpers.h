//
// Created by Aiken Wong on 5/2/23.
//

#ifndef SPA_PKBTESTHELPERS_H
#define SPA_PKBTESTHELPERS_H

#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

enum class testEntityType {
    TEST_ASSIGN_STATEMENT = 0,
    TEST_CALL_STATEMENT = 1,
    TEST_IF_STATEMENT = 2,
    TEST_PRINT_STATEMENT = 3,
    TEST_READ_STATEMENT = 4,
    TEST_WHILE_STATEMENT = 5,
    TEST_CONSTANT = 6,
    TEST_VARIABLE = 7,
    TEST_PROCEDURE = 8,
    TEST_STATEMENT = 9,
};
enum testRelationshipType {
    TEST_MODIFIES = 0,
    TEST_USES = 1,
    TEST_PARENT = 2,
    TEST_FOLLOWS = 3,
    TEST_PARENT_STAR = 4,
    TEST_FOLLOWS_STAR = 5,
    TEST_CALLS = 6,
    TEST_CALLS_STAR = 7,
};

extern int testBase;
extern std::string ONE_STRING_TEST_VALUE;
extern std::string TWO_STRING_TEST_VALUE;
extern int ONE_INT_TEST_VALUE;
extern int TWO_INT_TEST_VALUE;
extern std::string VARIABLE_TEST_VALUE;
extern std::string PROCEDURE_TEST_VALUE;
extern std::string PROCEDURE_TEST_VALUE_2;

class PKBtestHelpers {
   public:
    static bool relationshipEqualsRelationship(Relationship *relationship1, Relationship *relationship2);
};

#endif  // SPA_PKBTESTHELPERS_H
