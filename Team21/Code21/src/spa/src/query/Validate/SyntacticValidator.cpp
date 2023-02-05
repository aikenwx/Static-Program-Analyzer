#include "SyntacticValidator.h"

namespace qps {

	SyntacticValidator::SyntacticValidator(Query query) :QueryValidator(query) {}

	bool SyntacticValidator::validateQuery() {
		checkSuchThatCorrectRefTypes();
		checkAssignPatternCorrectRefTypes();
	}

	void SyntacticValidator::checkSuchThatCorrectRefTypes() {

	}

	void SyntacticValidator::checkAssignPatternCorrectRefTypes() {

	}
}