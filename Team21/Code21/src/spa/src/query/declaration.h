#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "query/design_entity.h"
#include "query/synonym.h"
#include <optional>

namespace qps {

class Declaration {

	private:
		const DesignEntity design_entity;
		const Synonym synonym;

	public:
		DesignEntity getDesignEntity();
		Synonym getSynonym();
		Declaration(DesignEntity design_entity_, Synonym synonym_);

		bool operator==(const Declaration& declaration2) const {
			return design_entity == declaration2.design_entity && synonym == declaration2.synonym;
		}

		friend std::ostream& operator<<(std::ostream& os, Declaration const& declaration) {
			os << getStringFromDE(declaration.design_entity) << " " << declaration.synonym;
			return os;
		}


		static std::optional<Declaration> Declaration::findDeclarationWithSynonym(std::vector<Declaration>& decl_list, Synonym& synonym) {
			return findDeclarationWithString(decl_list, synonym.getSynonym());
		}

		static std::optional<Declaration> Declaration::findDeclarationWithString(std::vector<Declaration>& decl_list, std::string& syn_string) {
			for (int i = 0; i < decl_list.size(); i++) {
				if (decl_list[i].getSynonym().getSynonym() == syn_string) {
					return std::make_optional<Declaration>(decl_list[i]);
				}
			}
			return std::nullopt;
		}
};
}
