#pragma once

#include "query/query.h"

namespace qps {

	class QueryValidator {
	private:
		Query query_;

	public:
		QueryValidator(Query query_) :query_(query_) {}

		virtual bool validateQuery() = 0;

		Query getQuery() {
			return query_;
		}
	};
}
