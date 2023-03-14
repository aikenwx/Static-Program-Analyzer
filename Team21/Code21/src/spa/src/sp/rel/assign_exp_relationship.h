#pragma once

#include <string>

#include "../ast/assign_node.h"
#include "relationship.h"

namespace rel {
class AssignExpRelationship : public Relationship {
 public:
  static std::unique_ptr<AssignExpRelationship> CreateRelationship(std::shared_ptr<ast::AssignNode> assignNode, std::string postfixExp);
  std::shared_ptr<ast::AssignNode> assignNode() const;
  std::string varName() const;
  int statementNumber() const;
  std::string postfixExp() const;
  RelationshipType relationshipType() const override { return RelationshipType::PROC; };

 private:
  AssignExpRelationship(std::shared_ptr<ast::AssignNode> assignNode, const std::string& postfixExp);
  std::shared_ptr<ast::AssignNode> assignNode_;
  std::string postfixExp_;
};
} // namespace rel
