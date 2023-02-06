#pragma once

#include "query/ref.h"
#include "query/relationship.h"
#include "query/declaration.h"

#include <vector>
#include <variant>

namespace qps {

class SuchThatClause {
	private:
		Relationship relationship;
		Ref arg1;
		Ref arg2;

		// Since Modifies and Uses are split into 2 types S and P
		void handleModifiesUses(std::vector<Declaration>& declarations) {
			if (relationship != Relationship::Uses && relationship != Relationship::Modifies) {
				return;
			}
			//Handle with synonym case first
			if (std::holds_alternative<Synonym>(arg1)) {
				auto synonym = std::get_if<Synonym>(&arg1);
				auto declaration = Declaration::findDeclarationWithSynonym(declarations, *synonym);
				if (declaration->getDesignEntity() == DesignEntity::PROCEDURE) {
					relationship = (relationship == Relationship::Uses)
						? Relationship::UsesP : Relationship::ModifiesP;
				} else {
					relationship = (relationship == Relationship::Uses)
						? Relationship::UsesS : Relationship::ModifiesS;
				}
			} else if (std::holds_alternative<QuotedIdentifier>(arg1)) {
				relationship = (relationship == Relationship::Uses)
					? Relationship::UsesP : Relationship::ModifiesP;
			} else if (std::holds_alternative<StatementNumber>(arg1)) {
				relationship = (relationship == Relationship::Uses)
					? Relationship::UsesP : Relationship::ModifiesP;
			}
		}

	public:
		Relationship getRelationship();
		Ref getArg1();
		Ref getArg2();
		SuchThatClause(Relationship relationship_, Ref arg1_, Ref arg2_, std::vector<Declaration>& declarations);

		bool operator==(const SuchThatClause& clause) const {
			return relationship == clause.relationship && arg1 == clause.arg1 &&
				arg2 == clause.arg2;
		}
		friend std::ostream& operator<<(std::ostream& os, SuchThatClause const& clause) {
			os << getStringFromRelationship(clause.relationship);
			return os;
		}

};
}
