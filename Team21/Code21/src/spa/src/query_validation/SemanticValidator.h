#pragma once

#include "query/query_exceptions.h"

#include "QueryValidator.h"
#include "query/relationship.h"

namespace qps {

class SemanticValidator : public QueryValidator {
 public:
	explicit SemanticValidator(Query& query);
	auto validateQuery() -> bool override;
	void checkForDuplicateDeclarations();
	void checkIfSynonymContainedInDeclaration();
	void checkPatternClauseSynonym();
	void checkNoWildCardFirstArgModifiesUses();
	void checkRelationSynonymMatchDesignEntity();
	void checkPatternSynonymMatchDesignEntity();
	void checkWithClauseSameAttributeCompare();
	void checkAttrRefValidAttrName();

 private:
	static void checkSynonymDeclareHelper(Ref reff, std::vector<Declaration>& declr, const std::string &missing);
	static void checkElementSynonymDeclareHelper(Element ele, std::vector<Declaration>& declr);
	static void checkWithRefSynonymDeclareHelper(WithRef reff, std::vector<Declaration>& declr);
	static void checkSynonymStatementHelper(DesignEntity des, const std::string &relStr);
	static void checkSecondSynonymHelper(DesignEntity des, const std::string &relStr, Relationship rel);
	static void checkAttrRefValidAttrNameHelper(AttrRef attr, std::vector<Declaration>& declr);
};

}  // namespace qps
