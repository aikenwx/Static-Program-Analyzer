#pragma once

#include "query/query_exceptions.h"

#include "QueryValidator.h"

namespace qps {

	//check if synonyms are IDENT is done in QueryParser
	class SyntacticValidator : public QueryValidator {
	public:
		SyntacticValidator(Query query);

		bool validateQuery();

		/*checks if such that clause relationship uses correct grammar.For example, stmtRef : synonym | '_' | INTEGER
		and entRef: synonym | '_' | '"' IDENT '"' so it essentially checks if type in relationship is corrrect*/
		void checkSuchThatCorrectRefTypes();

		//checks if assign pattern clause uses correct grammar. It checks if first arg is ent-ref and second ref is expression-spec
		void checkAssignPatternCorrectRefTypes();
	};
}
