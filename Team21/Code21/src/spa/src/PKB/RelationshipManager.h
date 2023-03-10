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
    std::unordered_map<RelationshipSynonymKey, std::shared_ptr<std::vector<Relationship*>>> relationshipSynonymStore;
    std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>> relationshipStore;

   public:
    RelationshipManager();

    ~RelationshipManager();

    void storeRelationship(Relationship* relationship);

    Relationship* getRelationship(RelationshipKey& key);

    std::vector<Relationship*>*
    getRelationshipsByTypes(RelationshipType& relationshipType, EntityType& leftHandEntityType,
                            EntityType& rightHandEntityType);

   private:
    void storeInRelationshipSynonymStore(Relationship* relationship);

    void initialiseVectorForRelationshipSynonymStoreIfNotExist(RelationshipSynonymKey relationshipSynonymKey);
};

#endif  // SPA_RELATIONSHIPMANAGER_H
