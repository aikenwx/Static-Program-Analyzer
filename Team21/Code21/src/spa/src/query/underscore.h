#pragma once

#include <iostream>

namespace qps {

    // Underscore class represents underscore as a type of stmtRef/entRef
    class Underscore {

    public:

        bool operator==(const Underscore& us) const {
            return true;
        }

        friend std::ostream& operator<<(std::ostream& os, Underscore const& us) {
            os << "_";
            return os;
        }
    };
}

