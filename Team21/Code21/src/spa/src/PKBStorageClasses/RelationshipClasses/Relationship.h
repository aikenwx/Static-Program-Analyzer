#ifndef SPA_RELATIONSHIP_H
#define SPA_RELATIONSHIP_H

#include "../EntityClasses/AssignStatement.h"
#include "../EntityClasses/Procedure.h"
#include "../StorageKey.h"

struct RelationshipType : public StorageKey {
   private:
    static std::size_t relationshipTypeKeyCounter;

   public:
    RelationshipType();
    bool operator==(const RelationshipType &relationshipType) const;
};

template <>
struct std::hash<RelationshipType> {
    std::size_t operator()(const RelationshipType &relationshipType) const;
};

struct RelationshipKey : public StorageKey {
   private:
    RelationshipType *relationshipType;
    EntityKey *leftEntityKey;
    EntityKey *rightEntityKey;

   public:
    RelationshipKey(RelationshipType *relationshipType, EntityKey *leftEntityKey, EntityKey *rightEntityKey);
    bool operator==(const RelationshipKey &otherRelationshipLiteralKey) const;
};

template <>
struct std::hash<RelationshipKey> {
    std::size_t operator()(const RelationshipKey &relationshipKey) const;
};

class Relationship {
   private:
    RelationshipKey relationshipKey;
    Entity *leftHandEntity;
    Entity *rightHandEntity;

   public:
    Relationship(RelationshipType *relationshipType, Entity *leftHandEntity, Entity *rightHandEntity);

    virtual ~Relationship() = default;

    RelationshipKey &getRelationshipKey();

    bool containsEntityOnLeftHand(Entity *entity);

    bool containsEntityOnRightHand(Entity *entity);

    virtual RelationshipType &getRelationshipType() const = 0;

    Entity *getLeftHandEntity() const;

    Entity *getRightHandEntity() const;

    bool equals(Relationship *otherRelationship);
};

#endif  // SPA_RELATIONSHIP_H
