#pragma once

#include "query/query_exceptions.h"
#include "QueryValidator.h"

#include <regex>

namespace qps {

	//check if synonyms are IDENT is done in QueryParser
	class SyntacticValidator : public QueryValidator {
	public:
		SyntacticValidator(Query query);

		bool validateQuery();

		bool isFactorValidInteger(std::string str);

		bool isValidFactor(std::string str);

		/*checks if such that clause relationship uses correct grammar.For example, stmtRef : synonym | '_' | INTEGER
		and entRef: synonym | '_' | '"' IDENT '"' so it essentially checks if type in relationship is corrrect*/
		void checkSuchThatCorrectRefTypes();

		//checks if assign pattern clause uses correct grammar. It checks if first arg is ent-ref and second ref is expression-spec
		void checkAssignPatternCorrectRefTypes();
	};
}
