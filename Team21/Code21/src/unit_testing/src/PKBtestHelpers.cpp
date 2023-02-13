//
// Created by Aiken Wong on 5/2/23.
//

#include "PKBtestHelpers.h"

#include <string>

int testBase = 10;

std::string ONE_STRING_TEST_VALUE = "1";
std::string TWO_STRING_TEST_VALUE = "2";
int ONE_INT_TEST_VALUE = 1;
int TWO_INT_TEST_VALUE = 2;
std::string VARIABLE_TEST_VALUE = "variable";
std::string PROCEDURE_TEST_VALUE = "procedure";

bool PKBtestHelpers::relationshipEqualsRelationship(Relationship *relationship1, Relationship *relationship2) {
    return relationship1->getRelationshipType() == relationship2->getRelationshipType() &&
           relationship1->getLeftHandEntity()->equals(relationship2->getLeftHandEntity()) &&
           relationship1->getRightHandEntity()->equals(relationship2->getRightHandEntity());
}
