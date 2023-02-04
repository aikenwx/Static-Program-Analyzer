//
// Created by Aiken Wong on 4/2/23.
//

#ifndef SPA_RELATIONSHIPHASHFACTORY_H
#define SPA_RELATIONSHIPHASHFACTORY_H

#include <string>
#include <array>
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"



class RelationshipHashFactory {
private:

    std::array<std::string, 4> relationshipTypeMapping = {"modifies", "uses", "parent", "follows"};

    std::array<std::string, 9> entityTypeMapping = {"Assign", "While", "If", "Print", "Read", "While", "Constant", "Variable", "Procedure"};


public:

    RelationshipHashFactory();

    std::string getHashKey(Relationship *relationship);

    std::string getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType, EntityType rightHandEntityType);

};


#endif //SPA_RELATIONSHIPHASHFACTORY_H
