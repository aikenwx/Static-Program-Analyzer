//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPMANAGER_H
#define SPA_RELATIONSHIPMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"


class RelationshipManager {

private:
    std::unordered_map<std::string, std::vector<Relationship *> *> *relationshipMap;

public:
    RelationshipManager();

    ~RelationshipManager();

    void storeRelationship(Relationship *relationship);

    std::vector<Relationship *> *
    getRelationshipsByTypes(RelationshipType relationshipType, EntityType leftHandEntityType,
                            EntityType rightHandEntityType);



private:
    void initialiseVectorForIndexIfNotExist(std::string hashkey);

    std::vector<std::string> *getPossibleHashKeysForGivenEntityAndRelationshipTypes(RelationshipType relationshipType,
                                                                                    EntityType leftHandEntityType,
                                                                                    EntityType rightHandEntityType);

};


#endif //SPA_RELATIONSHIPMANAGER_H
