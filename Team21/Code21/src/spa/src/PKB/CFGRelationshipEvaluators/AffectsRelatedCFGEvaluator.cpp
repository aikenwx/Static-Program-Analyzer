//
// Created by Aiken Wong on 31/3/23.
//

#include "AffectsRelatedCFGEvaluator.h"

#include "PKB/RelationshipManager.h"

AffectsRelatedCFGEvaluator::AffectsRelatedCFGEvaluator(cfg::CFG *cfg,
                                                       RelationshipStorage *relationshipStorage,
                                                       EntityManager *entityManager)
    : CFGRelationshipEvaluator(cfg, relationshipStorage, entityManager) {}

auto AffectsRelatedCFGEvaluator::isValidEntityInput(Entity *entity) -> bool {
  return entity->getEntityType() == AssignStatement::getEntityTypeStatic();
}

auto AffectsRelatedCFGEvaluator::isValidEntityTypeInput(const EntityType &entityType) -> bool {
  getEntityManager()->getEntitiesByType(entityType);
  return entityType == AssignStatement::getEntityTypeStatic() ||
      entityType == Statement::getEntityTypeStatic();
}

auto AffectsRelatedCFGEvaluator::getEvaluableEntitiesFromEntityType(
    const EntityType &entityType) -> std::vector<Entity *> * {
  if (!isValidEntityTypeInput(entityType)) {
    return RelationshipManager::getEmptyEntityVector();
  }

  return getEntityManager()->getEntitiesByType(AssignStatement::getEntityTypeStatic());
}

auto AffectsRelatedCFGEvaluator::isAllowedModifier(Entity *entity) -> bool {
  return entity->getEntityType() == ReadStatement::getEntityTypeStatic() || entity->getEntityType() == CallStatement::getEntityTypeStatic() ||
  entity->getEntityType() == AssignStatement::getEntityTypeStatic();
}
