#pragma once

#include <string>

#include "../ast/assign_node.h"
#include "relationship.h"

namespace rel {
class AssignExpRelationship : public Relationship {
 public:
  static std::unique_ptr<AssignExpRelationship> CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode);
  std::shared_ptr<ast::AssignNode> assignNode();
  std::string varName();
  int statementNumber();
  std::string postfixExp_();
  RelationshipType relationshipType() override { return RelationshipType::PROC; };

 private:
  AssignExpRelationship(std::shared_ptr<ast::AssignNode> assignNode);
  std::shared_ptr<ast::AssignNode> assignNode_;
  std::string postfixExp_;
};
} // namespace rel
