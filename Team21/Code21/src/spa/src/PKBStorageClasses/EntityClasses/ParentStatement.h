//
// Created by Aiken Wong on 3/4/23.
//

#ifndef SPA_PARENTSTATEMENT_H
#define SPA_PARENTSTATEMENT_H

#include "Statement.h"

class ParentStatement : public Statement {
 public:
  ParentStatement(const EntityType* entityType, int statementNumber);

  auto operator=(const ParentStatement& parentStatement) -> ParentStatement& = default;

  ParentStatement(const ParentStatement& parentStatement) = default;

  auto operator=(ParentStatement&& parentStatement) noexcept -> ParentStatement& = default;

  ParentStatement(ParentStatement&& parentStatement) noexcept = default;

  static auto isParentStatement(Entity* entity) -> bool;

  ~ParentStatement() override = default;
};

#endif  // SPA_PARENTSTATEMENT_H
