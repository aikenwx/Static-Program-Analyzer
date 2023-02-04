#include "query_preprocess/query_parser.h"

namespace qps {

	QueryParser::QueryParser(std::vector<std::string> tokens) : tokens_{ tokens } {}

	bool QueryParser::dummyParse(int num)
	{
		if (num == 1)
			return true;
		else
			return false;
	}
}
