#pragma once

#include <string>
#include<vector>

#include "query/query.h"
#include "query_preprocess/query_tokenizer.h"
#include "query_preprocess/query_parser.h"

namespace qps {
	class QueryHelper {
	public:
		static Query buildQuery(std::string str) {
			std::string dupeInput(str);
			qps::QueryTokenizer tokenizer(dupeInput);
			std::vector<std::string> tokenList = tokenizer.tokenize();
			qps::QueryParser parser(tokenList);
			return parser.parse();
		}
	};
}
