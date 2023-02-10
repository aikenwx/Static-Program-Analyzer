//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

class RelationshipManager {
   private:
    std::unordered_map<int, std::shared_ptr<std::vector<std::shared_ptr<Relationship>>>> relationshipMap;

   public:
    RelationshipManager();

    ~RelationshipManager();

    void storeRelationship(Relationship *relationship);

    // ownership of the vector is transferred to the caller, so we return a smart pointer
    std::vector<std::shared_ptr<Relationship>> *
    getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType,
                            EntityType rightHandEntityType);

   private:
    void initialiseVectorForIndexIfNotExist(int hashkey);
    std::shared_ptr<std::vector<int>> getPossibleHashKeysForGivenEntityAndRelationshipTypes(RelationshipType relationshipType,
                                                                                            EntityType leftHandEntityType,
                                                                                            EntityType rightHandEntityType);
};

#endif  // SPA_RELATIONSHIPMANAGER_H
