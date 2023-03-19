#pragma once

#include "query/query.h"

namespace qps {

	class QueryValidator {
	private:
		Query& query_;

	public:

		explicit QueryValidator(Query& query_) :query_(query_) {}

		virtual ~QueryValidator() = default;

		QueryValidator(const QueryValidator&) = default;

		auto operator=(const QueryValidator&) -> QueryValidator& = delete;

		QueryValidator(QueryValidator&&) = default;

		auto operator=(QueryValidator&&) -> QueryValidator& = delete;

		virtual auto validateQuery() -> bool = 0;

		[[nodiscard]] auto getQuery() const -> Query& {
			return query_;
		}
	};
}  // namespace qps
