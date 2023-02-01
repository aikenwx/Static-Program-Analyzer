#include "query/declaration.h"

#include <vector>
#include <optional>

namespace qps {

	Declaration::Declaration(DesignEntity design_entity_, Synonym synonym_)
		: design_entity{ design_entity_ }, synonym{ synonym_ } {}

	DesignEntity Declaration::getDesignEntity() {
		return design_entity;
	}

	Synonym Declaration::getSynonym() {
		return synonym;
	}
}