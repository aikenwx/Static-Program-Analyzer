//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

struct RelationshipDoubleSynonymKey : public StorageKey {
   private:
    RelationshipType *relationshipType;
    EntityType *leftHandEntityType;
    EntityType *rightHandEntityType;

   public:
    RelationshipDoubleSynonymKey(RelationshipType *relationshipType, EntityType *leftHandEntityType, EntityType *rightHandEntityType);
    bool operator==(const RelationshipDoubleSynonymKey &otherRelationshipSynonymKey) const;
};

template <>
struct std::hash<RelationshipDoubleSynonymKey> {
    std::size_t operator()(const RelationshipDoubleSynonymKey &relationshipSynonymKey) const;
};

struct RelationshipSynonymLiteralKey : public StorageKey {
   private:
    RelationshipType *relationshipType;
    EntityType *leftHandEntityType;
    EntityKey *rightHandEntityKey;

   public:
    RelationshipSynonymLiteralKey(RelationshipType *relationshipType, EntityType *entityType, EntityKey *entityKey);
    bool operator==(const RelationshipSynonymLiteralKey &otherRelationshipLiteralSynonymKey) const;
};

template <>
struct std::hash<RelationshipSynonymLiteralKey> {
    std::size_t operator()(const RelationshipSynonymLiteralKey &relationshipLiteralSynonymKey) const;
};

struct RelationshipLiteralSynonymKey : public StorageKey {
   private:
    RelationshipType *relationshipType;
    EntityKey *leftHandEntityKey;
    EntityType *rightHandEntityType;

   public:
    RelationshipLiteralSynonymKey(RelationshipType *relationshipType, EntityKey *entityKey, EntityType *entityType);
    bool operator==(const RelationshipLiteralSynonymKey &otherRelationshipLiteralSynonymKey) const;
};

template <>
struct std::hash<RelationshipLiteralSynonymKey> {
    std::size_t operator()(const RelationshipLiteralSynonymKey &relationshipLiteralSynonymKey) const;
};

class RelationshipManager {
   private:
    std::unordered_map<RelationshipDoubleSynonymKey, std::shared_ptr<std::vector<Relationship *>>> relationshipDoubleSynonymStore;

    std::unordered_map<RelationshipSynonymLiteralKey, std::shared_ptr<std::vector<Entity *>>> relationshipSynonymLiteralStore;

    std::unordered_map<RelationshipSynonymLiteralKey, std::shared_ptr<std::vector<Entity *>>> relationshipSynonymLiteralVectorStore;

    std::unordered_map<RelationshipLiteralSynonymKey, std::shared_ptr<std::vector<Entity *>>> relationshipLiteralSynonymStore;

    std::unordered_map<RelationshipKey, std::shared_ptr<Relationship>> relationshipStore;

   public:
    RelationshipManager();

    void storeRelationship(Relationship *relationship);

    Relationship *getRelationship(RelationshipKey &key);

    std::vector<Relationship *> *
    getRelationshipsByTypes(RelationshipType &relationshipType, EntityType &leftHandEntityType,
                            EntityType &rightHandEntityType);

    std::vector<Entity *> *
    getEntitiesForGivenRelationshipTypeAndLeftHandEntityType(RelationshipType &relationshipType, EntityType &leftHandEntityType,
                                                             EntityKey &rightHandEntityKey);
    std::vector<Entity *> *
    getEntitiesForGivenRelationshipTypeAndRightHandEntityType(RelationshipType &relationshipType, EntityKey &leftHandEntityKey,
                                                              EntityType &rightHandEntityType);

   private:
    void storeInRelationshipDoubleSynonymStore(Relationship *relationship);

    void storeInRelationshipSynonymLiteralStore(Relationship *relationship);

    void storeInRelationshipLiteralSynonymStore(Relationship *relationship);

    void initialiseVectorForRelationshipDoubleSynonymStoreIfNotExist(RelationshipDoubleSynonymKey relationshipSynonymKey);

    void initialiseVectorForRelationshipLiteralSynonymStoreIfNotExist(RelationshipLiteralSynonymKey relationshipSynonymKey);

    void initialiseVectorForRelationshipSynonymLiteralStoreIfNotExist(RelationshipSynonymLiteralKey relationshipSynonymKey);
};

#endif  // SPA_RELATIONSHIPMANAGER_H
