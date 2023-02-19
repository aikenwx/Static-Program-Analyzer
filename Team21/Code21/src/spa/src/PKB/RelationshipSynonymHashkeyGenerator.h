//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPSYNONYMHASHKEYGENERATOR_H
#define SPA_RELATIONSHIPSYNONYMHASHKEYGENERATOR_H

#include <string>
#include <array>
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"


class RelationshipSynonymHashkeyGenerator {
private:

public:

    RelationshipSynonymHashkeyGenerator() = default;

    int getHashKey(Relationship *relationship);

    int getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, EntityType rightHandEntityType);

};


#endif //SPA_RELATIONSHIPSYNONYMHASHKEYGENERATOR_H
