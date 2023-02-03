#pragma once

#include <iostream>

namespace qps {

    // Class QuotedIdentifier represents a quoted identifier in stmtRef or entRef.
    class QuotedIdentifier {
    private:
        std::string id;

    public:
        QuotedIdentifier(std::string id_);
        std::string getQuotedId();

        static bool isValidId(std::string id);

        bool operator==(QuotedIdentifier const& quote_id2) const {
            return id == quote_id2.id;
        }

        friend std::ostream& operator<<(std::ostream& os, QuotedIdentifier const& quote_id) {
            os << quote_id.id;
            return os;
        }
    };
}
