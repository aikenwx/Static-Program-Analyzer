//
// Created by Aiken Wong on 11/2/23.
//

#ifndef SPA_RELATIONSHIPLITERALHASHKEYGENERATOR_H
#define SPA_RELATIONSHIPLITERALHASHKEYGENERATOR_H

#include <array>
#include <string>

#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

class RelationshipLiteralHashkeyGenerator {
   private:
   public:
    RelationshipLiteralHashkeyGenerator() = default;

    std::string getHashKey(Relationship* relationship);
    std::string getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, std::string* leftEntityValue, EntityType rightHandEntityType, std::string* rightEntityValue);
};

#endif  // SPA_RELATIONSHIPLITERALHASHKEYGENERATOR_H
