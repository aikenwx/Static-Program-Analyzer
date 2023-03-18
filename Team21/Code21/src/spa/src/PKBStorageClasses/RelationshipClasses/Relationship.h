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
    auto operator==(const RelationshipType &relationshipType) const -> bool;
};

template <>
struct std::hash<RelationshipType> {
  auto operator()(const RelationshipType &relationshipType) const
      -> std::size_t;
};

struct RelationshipKey : public StorageKey {
   private:
    const RelationshipType *relationshipType;
    EntityKey *leftEntityKey;
    EntityKey *rightEntityKey;

   public:
    RelationshipKey(const RelationshipType *relationshipType, EntityKey *leftEntityKey, EntityKey *rightEntityKey);
    auto operator==(const RelationshipKey &otherRelationshipLiteralKey) const
        -> bool;
};

template <>
struct std::hash<RelationshipKey> {
  auto operator()(const RelationshipKey &relationshipKey) const -> std::size_t;
};

class Relationship {
   private:
    RelationshipKey relationshipKey;
    Entity *leftHandEntity;
    Entity *rightHandEntity;

   public:
    Relationship(const RelationshipType *relationshipType, Entity *leftHandEntity, Entity *rightHandEntity);

    virtual ~Relationship() = default;

    auto getRelationshipKey() -> RelationshipKey &;

    auto containsEntityOnLeftHand(Entity *entity) const -> bool;

    auto containsEntityOnRightHand(Entity *entity) const -> bool;

    [[nodiscard]] virtual auto getRelationshipType() const
        -> const RelationshipType & = 0;

    [[nodiscard]] auto getLeftHandEntity() const -> Entity *;

    [[nodiscard]] auto getRightHandEntity() const -> Entity *;

    auto equals(Relationship *otherRelationship) -> bool;
};

#endif  // SPA_RELATIONSHIP_H
