#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <iostream>
#include <string>

#include "exceptions/syntax_error.h"
#include "sp/ast/astlib.h"
#include "sp/ast/program_node.h"
#include "sp/parser/simple_chain_parser.h"
#include "sp/tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace parser {
using Catch::Matchers::Message;

tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
SimpleChainParser parser = SimpleChainParser();

template<typename T>
auto CheckRootIsNodeType(std::string const &input) -> bool {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(input);
  auto pair = parser.Parse(std::move(tokens));
  return util::instance_of<T>(pair.second->GetRoot());
}

auto CheckRootIsValidProgram(std::string const &program) -> bool {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(program);
  auto pair = parser.Parse(std::move(tokens));
  if (!pair.first) {
    return false;
  }
  return util::instance_of<ast::ProgramNode>(pair.second->GetRoot());
}

auto CheckStatementCount(std::string const &program, int count) -> bool {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(program);
  std::shared_ptr<ast::INode> root = parser.Parse(std::move(tokens)).second->GetRoot();
  return std::static_pointer_cast<ast::ProgramNode>(root)->GetTotalStatementCount() == count;
}

/*
* General program test cases
*/
TEST_CASE("Parser throws a SyntaxError with an empty program",
  "[Parser]") {
  std::string program = "";
  REQUIRE_THROWS_MATCHES(CheckRootIsValidProgram(program), exceptions::SyntaxError,
    Message("Syntax error: Empty program"));
};

TEST_CASE(
  "Parser correctly parses a valid program with a read statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with a print statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    print x;
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with an assigns statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    x = 2;
  })";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with one procedure and one call statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    call hello;
  })";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with multiple procedures and call statements",
  "[Parser]") {
  std::string program = R"(procedure hello {
    call world;
  }
  procedure world {
    read x;
  })";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with an if statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    if (x > 0) then {
      read x;
    } else {
      read x;
    }
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with a while statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (x > 0) {
      read x;
    }
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with nested if statements",
  "[Parser]") {
  std::string program = R"(procedure hello {
    if (x > 0) then {
      if (a == 3) then {
        read a;
      } else {
        if (c > 4) then {
          a = 3 * 2;
        } else {
          x = 1;
        }
      }
    } else {
      if (y < 2) then {
        print y;
      } else {
        z = 3;
      }
    }
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with nested while statements",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (x < 0) {
      read a;
      while (y > 0) {
        z = z + 1;
        while (z < 0) {
          print a;
        }
      }
    }
})";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with one procedure",
  "[Parser]") {
  std::string program = R"(
      procedure main {
        a = 1;
        b = 2;
        print x;
        read y;
      })";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with multiple procedures",
  "[Parser]") {
  std::string program = R"(procedure main {
      x = 1;
      read y;
      print z;
      call foo;
    }
    procedure foo {
      z = 2;
      call bar;
    }
    procedure bar {
      x = 1;
      if (x == 1) then {
        call baz;
      } else {
        call qux;
      }
    }
    procedure baz {
      x = 4;
    }
    procedure qux {
      print w;
    }
    procedure quux {
      call main;
    })";
  REQUIRE(CheckRootIsValidProgram(program));
};

TEST_CASE(
  "Parser doesn't parse a valid ProgramNode when parsing a program with one procedure with invalid if-statement syntax",
  "[Parser]") {
  std::string program = R"(procedure hello {
    if (x > 0) {
      print positive;
    } else {
      read nonpositive;
    }
  })";
  REQUIRE_FALSE(CheckRootIsValidProgram(program));
}

TEST_CASE(
  "Parser doesn't parse a valid ProgramNode when parsing a program with one valid procedure, followed by one procedure with invalid if-statement syntax",
  "[Parser]") {
  std::string program = R"(procedure dummy {
    read x;
  }

  procedure hello {
    if (x > 0) {
      print positive;
    } else {
      read nonpositive;
    }
  })";
  REQUIRE_FALSE(CheckRootIsValidProgram(program));
}

TEST_CASE(
  "Parser doesn't parse a valid ProgramNode when parsing a program with one valid procedure, followed by one procedure with invalid cond-expr syntax",
  "[Parser]") {
  std::string program = R"(procedure dummy {
    read x;
  }

  procedure hello {
    if (x == 1 && true) then {
      print positive;
    } else {
      read nonpositive;
    }
  })";
  REQUIRE_FALSE(CheckRootIsValidProgram(program));
}

/*
* Grammar test cases
*/
TEST_CASE(
  "Parser correctly parses constants",
  "[Parser]") {
  std::string input = R"(7)";
  REQUIRE(CheckRootIsNodeType<ast::ConstantNode>(input));
};

TEST_CASE(
  "Parser correctly parses identifiers",
  "[Parser]") {
  std::string input = R"(hello)";
  REQUIRE(CheckRootIsNodeType<ast::IdentifierNode>(input));
};

/*
* Expression test cases
*/
TEST_CASE(
  "Parser correctly parses expressions with one constant",
  "[Parser]") {
  std::string input = R"(4;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with one variable",
  "[Parser]") {
  std::string input = R"(z;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with addition",
  "[Parser]") {
  std::string input = R"(x + 2;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with subtraction",
  "[Parser]") {
  std::string input = R"(y - 1;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with multiplication",
  "[Parser]") {
  std::string input = R"(x * 2;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with division",
  "[Parser]") {
  std::string input = R"(z / 2;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with modulo",
  "[Parser]") {
  std::string input = R"(y % 10;)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses expressions with parentheses",
  "[Parser]") {
  std::string input = R"((a);)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses more complicated expressions",
  "[Parser]") {
  std::string input = R"((x + 2) + 2 * 4;)";
  CHECK(CheckRootIsNodeType<ast::ExpressionNode>(input));
  input = R"(((x + 2) + 4) * (((30 + ((y))))));)";
  CHECK(CheckRootIsNodeType<ast::ExpressionNode>(input));
  input = R"((((x))) % 3;)";
  CHECK(CheckRootIsNodeType<ast::ExpressionNode>(input));
  input = R"(((1 * (x) * 3)) * 4 + ((((((5))))) / (2);)";
  CHECK(CheckRootIsNodeType<ast::ExpressionNode>(input));
  input = R"(((x - ((3 * 2))) - 1);)";
  REQUIRE(CheckRootIsNodeType<ast::ExpressionNode>(input));
}

/*
* Assign test cases
*/
TEST_CASE(
  "Parser correctly parses assign",
  "[Parser]") {
  std::string input = R"(b = 1;)";
  REQUIRE(CheckRootIsNodeType<ast::AssignNode>(input));
};

TEST_CASE(
  "Parser correctly parses assign with expressions",
  "[Parser]") {
  std::string input = R"(b = ((1 + x));)";
  CHECK(CheckRootIsNodeType<ast::AssignNode>(input));
  input = R"(b = x * 5 + 4 / 2;)";
  CHECK(CheckRootIsNodeType<ast::AssignNode>(input));
  input = R"(b = (((x + y) / 7) + 0 * 5) - 2;)";
  REQUIRE(CheckRootIsNodeType<ast::AssignNode>(input));
};

/*
* Conditional expression test cases
*/
TEST_CASE(
  "Parser correctly parses conditional expressions with lesser",
  "[Parser]") {
  std::string input = R"(y < 2))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with greater",
  "[Parser]") {
  std::string input = R"(x > 0))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with equals",
  "[Parser]") {
  std::string input = R"(a == 2))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with lesser equals",
  "[Parser]") {
  std::string input = R"(y <= 5))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with greater equals",
  "[Parser]") {
  std::string input = R"(z >= 1))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with not equals",
  "[Parser]") {
  std::string input = R"(b != 0))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with expressions on LHS",
  "[Parser]") {
  std::string input = R"((x + 2) < 0))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"(x + 2 < 0))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"((((x + 2))) < 0))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
}

TEST_CASE(
  "Parser correctly parses conditional expressions with expressions on RHS",
  "[Parser]") {
  std::string input = R"(y < (5 + x)))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"(y < 5 + x))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"(y < (((5 + x)))))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
}

TEST_CASE(
  "Parser correctly parses conditional expressions with expressions on LHS and RHS",
  "[Parser]") {
  std::string input = R"((x + 7) < (5 * a)))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"(x + 7 < 5 * a))";
  CHECK(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
  input = R"((((x + 7))) < (((5 * a)))))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
}

TEST_CASE(
  "Parser correctly parses conditional expressions with not",
  "[Parser]") {
  std::string input = R"(!(x < 0)))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with and",
  "[Parser]") {
  std::string input = R"((y < 2) && (z > 2)))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

TEST_CASE(
  "Parser correctly parses conditional expressions with or",
  "[Parser]") {
  std::string input = R"((x < 7) || (a > 3)))";
  REQUIRE(CheckRootIsNodeType<ast::ConditionalExpressionNode>(input));
};

/*
* Statement number test cases
*/
TEST_CASE(
  "Parser correctly assigns statement numbers to a simple valid program",
  "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
})";
  REQUIRE(CheckStatementCount(program, 1));
};

TEST_CASE(
  "Parser correctly assigns statement numbers to a longer valid program",
  "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
    print y;
    z = 3;
    read a;
    print b;
    c = 40;
})";
  REQUIRE(CheckStatementCount(program, 6));
};

TEST_CASE(
  "Parser correctly assigns statement numbers to a valid program with an if statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    read firstline;
    if (x > 0) then {
      print positive;
    } else {
      read nonpositive;
    }
})";
  REQUIRE(CheckStatementCount(program, 4));
};

TEST_CASE(
  "Parser correctly assigns statement numbers to a valid program with a while statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (y > 0) {
      x = y + 1;
    }
    print endline;
})";
  REQUIRE(CheckStatementCount(program, 3));
};

TEST_CASE(
  "Parser correctly assigns statement numbers to a valid program with nested if statements",
  "[Parser]") {
  std::string program = R"(procedure hello {
    if (x > 0) then {
      if (a == 3) then {
        read a;
      } else {
        if (c > 4) then {
          a = 3 * 2;
        } else {
          x = 1;
        }
      }
    } else {
      if (y < 2) then {
        print y;
      } else {
        z = 3;
      }
    }
})";
  REQUIRE(CheckStatementCount(program, 9));
};

TEST_CASE(
  "Parser correctly assigns statement numbers to a valid program with nested while statements",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (x < 0) {
      read a;
      while (y > 0) {
        z = z + 1;
        while (z < 0) {
          print a;
        }
      }
    }
})";
  REQUIRE(CheckStatementCount(program, 6));
};
}  // namespace parser
