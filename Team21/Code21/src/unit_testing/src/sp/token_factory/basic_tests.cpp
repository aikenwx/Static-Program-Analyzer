#include "catch.hpp"
#include "exceptions/syntax_error.h"
#include "token_factory/simple_token_factory.h"

namespace test_token_factory {
SCENARIO("Token factory should return valid Tokens for any valid token",
         "[sp][sp/token_factory]") {
  GIVEN("A valid token") {
    std::vector<std::string> validTokens = {
        "{",  "}",  "(", ")",  ";", "+",  "-", "*", "/",  "%",
        "==", "!=", "<", "<=", ">", ">=", "!", "=", "&&", "||"};

    for (auto token : validTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("Token is created") {
        THEN("Token should be valid") {
          REQUIRE_NOTHROW(token_factory.CreateToken(token));
        }
      }
    }
  }
}

SCENARIO("Token factory should handle integers appropriately",
         "[sp][sp/token_factory]") {
  GIVEN("A single-character zero integer") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("Token should be valid") {
        REQUIRE_NOTHROW(token_factory.CreateToken("0"));
      }
    }
  };

  GIVEN("An integer with no leading zeroes") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("Token should be valid") {
        REQUIRE_NOTHROW(token_factory.CreateToken("151"));
      }
    }
  }

  GIVEN("A multiple-character zero integer") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            token_factory.CreateToken("00"), exceptions::SyntaxError,
            Catch::Message(
                "Syntax error: Non-zero integer cannot start with 0"));
      }
    }
  };

  GIVEN("A non-zero integer with leading zeroes") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            token_factory.CreateToken("0151"), exceptions::SyntaxError,
            Catch::Message(
                "Syntax error: Non-zero integer cannot start with 0"));
      }
    }
  };
};
SCENARIO("Token factory should handle identifiers appropriately",
         "[sp][sp/token_factory]") {
  GIVEN("A zero-length identifier") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            token_factory.CreateToken(""), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid identifier"));
      }
    }
  };

  GIVEN("An identifier with a leading digit") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            token_factory.CreateToken("1abc"), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid identifier"));
      }
    }
  };

  GIVEN("An identifier with any non-alphanumeric character") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("A syntax error should be thrown") {
        REQUIRE_THROWS_MATCHES(
            token_factory.CreateToken("ab_c"), exceptions::SyntaxError,
            Catch::Message("Syntax error: Invalid identifier"));
      }
    }
  };

  GIVEN("A valid (alphanumeric) identifier") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("Token is created") {
      THEN("Token should be valid") {
        REQUIRE_NOTHROW(token_factory.CreateToken("a1bc4"));
      }
    }
  };
};

SCENARIO("TokenFactory should correctly identify all whitespaces",
         "[sp][sp/token_factory]") {
  GIVEN("A valid whitespace token") {
    std::vector<char> validWhitespaceTokens = {' ', '\t', '\r', '\n'};

    for (auto token : validWhitespaceTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("isWhitespace() is called") {
        THEN("isWhitespace should return true") {
          REQUIRE(token_factory.isWhitespace(token));
        }
      }
    }
  };

  GIVEN("An unsupported whitespace token") {
    std::vector<char> otherWhitespaceTokens = {'\v', '\f'};

    for (auto token : otherWhitespaceTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("isWhitespace() is called") {
        THEN("isWhitespace should return false") {
          REQUIRE_FALSE(token_factory.isWhitespace(token));
        }
      }
    }
  };

  GIVEN("A non-whitespace token") {
    std::vector<char> nonWhitespaceTokenExamples = {'a', '5', '_', '%', '='};

    for (auto token : nonWhitespaceTokenExamples) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("isWhitespace() is called") {
        THEN("isWhitespace should return false") {
          REQUIRE_FALSE(token_factory.isWhitespace(token));
        }
      }
    }
  };
};

SCENARIO("TokenFactory should correctly identify symbols",
         "[sp][sp/token_factory]") {
  std::vector<std::string> validSymbolTokens = {
      "{",  "}",  "(", ")",  ";", "+",  "-", "*", "/",  "%",
      "==", "!=", "<", "<=", ">", ">=", "!", "=", "&&", "||"};

  GIVEN("A valid symbol token") {
    for (auto token : validSymbolTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("checkSymbol() is called") {
        THEN("checkSymbol() should return {true, true}") {
          token_factory::CheckSymbolResult result =
              token_factory.checkSymbol(token);
          REQUIRE(result.isValidPrefix);
          REQUIRE(result.isValidSymbol);
        }
      }
    }
  };

  GIVEN("A valid symbol prefix") {
    for (auto token : validSymbolTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("checkSymbol() is called with a symbol prefix that isn't a symbol") {
        std::string prefix = token.substr(0, 1);
        if (prefix == token) {
          continue;
        }
        THEN("checkSymbol() should return {true, false}") {
          token_factory::CheckSymbolResult result =
              token_factory.checkSymbol(prefix);
          REQUIRE(result.isValidPrefix);
          REQUIRE_FALSE(result.isValidSymbol);
        }
      }
    }
  };

  GIVEN(
      "A string that isn't a valid symbol token, but has a prefix that is a "
      "valid symbol token") {
    for (auto token : validSymbolTokens) {
      token_factory::SimpleTokenFactory token_factory =
          token_factory::SimpleTokenFactory();
      WHEN("checkSymbol() is called with this string") {
        std::string invalidToken = token + token[-1];
        THEN("checkSymbol() should return {false, false}") {
          token_factory::CheckSymbolResult result =
              token_factory.checkSymbol(invalidToken);
          REQUIRE_FALSE(result.isValidPrefix);
          REQUIRE_FALSE(result.isValidSymbol);
        }
      }
    }
  };

  GIVEN("A string that doesn't have a prefix that is a valid symbol prefix") {
    token_factory::SimpleTokenFactory token_factory =
        token_factory::SimpleTokenFactory();
    WHEN("checkSymbol() is called with this string") {
      std::string invalidToken = "a" + validSymbolTokens[0];
      THEN("checkSymbol() should return {false, false}") {
        token_factory::CheckSymbolResult result =
            token_factory.checkSymbol(invalidToken);
        REQUIRE_FALSE(result.isValidPrefix);
        REQUIRE_FALSE(result.isValidSymbol);
      }
    }
  };
};
}  // namespace test_token_factory
