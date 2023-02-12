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
    std::string getHashKey(RelationshipType relationshipType, EntityType leftHandEntityType,  std::string* leftEntityValue, EntityType rightHandEntityType,  std::string* rightEntityValue);

    std::string getStatementModifiesVariableHashKey(int statementNumber, std::string* variableName);
    std::string getProcedureModifiesVariableHashKey(std::string* procedureName, std::string* variableName);
    std::string getStatementUsesVariableHashKey(int statementNumber, std::string* variableName);
    std::string getProcedureUsesVariableHashKey(std::string* procedureName, std::string* variableName);
    std::string getParentRelationshipHashKey(int parentStatementNumber, int childStatementNumber);
    std::string getFollowsRelationshipHashKey(int firstStatementNumber, int secondStatementNumber);
    std::string getFollowsStarRelationshipHashKey(int firstStatementNumber, int secondStatementNumber);
    std::string getParentStarRelationshipHashKey(int parentStatementNumber, int childStatementNumber);
};

#endif  // SPA_RELATIONSHIPLITERALHASHKEYGENERATOR_H
