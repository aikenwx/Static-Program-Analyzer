//
// Created by Aiken Wong on 5/2/23.
//

#ifndef SPA_PKBTESTHELPERS_H
#define SPA_PKBTESTHELPERS_H

#include "PKBStorageClasses/RelationshipClasses/Relationship.h"


class PKBtestHelpers {
public:
    static bool relationshipEqualsRelationship(Relationship *relationship1, Relationship *relationship2);
};


#endif //SPA_PKBTESTHELPERS_H
