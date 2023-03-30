//
// Created by Aiken Wong on 25/3/23.
//

#ifndef SPA_NEXTSTARRELATIONSHIP_H
#define SPA_NEXTSTARRELATIONSHIP_H

#include "Relationship.h"
#include "CFGEvaluatableRelationshipType.h"
#include "PKB/CFGRelationshipEvaluators/NextStarCFGEvaluator.h"



class NextStarRelationshipType : public CFGEvaluatableRelationshipType {

  public:
    auto getRelationshipEvaluator(cfg::CFG* cfg, RelationshipStorage* relationshipStorage, EntityManager* entityManager) const -> std::shared_ptr<CFGRelationshipEvaluator> override;
};

class NextStarRelationship : public Relationship {
  private:
    static const NextStarRelationshipType relationshipType;
  
  public:
    static auto getRelationshipTypeStatic() -> const RelationshipType&;
  
    NextStarRelationship(Statement* prevStatement, Statement* nextStatement);
  
    [[nodiscard]] auto getRelationshipType() const -> const RelationshipType& override;
  };

  


#endif //SPA_NEXTSTARRELATIONSHIP_H
