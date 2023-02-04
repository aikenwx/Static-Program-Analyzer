#include "query_preprocess/query_tokenizer.h"

namespace qps {

	QueryTokenizer::QueryTokenizer(std::string source) : currentStr{ source } {}

	bool QueryTokenizer::dummyTokenize(int num)
	{
		if (num == 1)
			return true;
		else
			return false;
	}
}
