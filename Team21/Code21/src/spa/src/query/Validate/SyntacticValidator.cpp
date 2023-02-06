#include "SyntacticValidator.h"

namespace qps {

	SyntacticValidator::SyntacticValidator(Query query) :QueryValidator(query) {}

	bool SyntacticValidator::validateQuery() {
		checkSuchThatCorrectRefTypes();
		checkAssignPatternCorrectRefTypes();
		return true;
	}

	void SyntacticValidator::checkSuchThatCorrectRefTypes() {

	}

	void SyntacticValidator::checkAssignPatternCorrectRefTypes() {

	}
}
