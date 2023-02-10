//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPHASHKEYFACTORY_H
#define SPA_RELATIONSHIPHASHKEYFACTORY_H

#include <string>
#include <array>
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"


class RelationshipHashkeyFactory {
private:

public:

    RelationshipHashkeyFactory();

    int getHashKey(Relationship *relationship);

    int getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, EntityType rightHandEntityType);

};


#endif //SPA_RELATIONSHIPHASHKEYFACTORY_H
