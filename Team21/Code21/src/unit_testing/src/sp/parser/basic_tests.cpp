#include <iostream>
#include <string>

#include "catch.hpp"
#include "exceptions/syntax_error.h"
#include "sp/ast/astlib.h"
#include "sp/parser/simple_parser.h"
#include "tokenizer/simple_tokenizer.h"
#include "util/instance_of.h"

namespace parser {
tokenizer::SimpleTokenizer tokenizer = tokenizer::SimpleTokenizer();
SimpleParser parser = SimpleParser();

template<typename T>
bool CheckRootIsNodeType(std::string input) {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(input);
  std::shared_ptr<ast::INode> root = parser.Parse(std::move(tokens))->GetRoot();
  return util::instance_of<T>(root);
}

bool CheckRootIsProgram(std::string program) {
  return CheckRootIsNodeType<ast::ProgramNode>(program);
}

bool CheckStatementCount(std::string program, int count) {
  std::vector<std::unique_ptr<token::Token>> tokens =
    tokenizer.tokenize(program);
  std::shared_ptr<ast::INode> root = parser.Parse(std::move(tokens))->GetRoot();
  return std::static_pointer_cast<ast::ProgramNode>(root)->GetTotalStatementCount() == count;
}

/*
* General program test cases
*/
TEST_CASE("Parser throws a SyntaxError with an empty program",
  "[Parser]") {
  std::string program = "";
  REQUIRE_THROWS_MATCHES(CheckRootIsProgram(program), exceptions::SyntaxError,
    Catch::Message("Syntax error: Empty program"));
};

TEST_CASE(
  "Parser parses a program with no statements without throwing an error",
  "[Parser]") {
  std::string program = R"(procedure hello {
})";
  REQUIRE_NOTHROW(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with a read statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    read x;
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with a print statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    print x;
})";
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with an assigns statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    x = 2;
})";
  REQUIRE(CheckRootIsProgram(program));
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
  REQUIRE(CheckRootIsProgram(program));
};

TEST_CASE(
  "Parser correctly parses a valid program with a while statement",
  "[Parser]") {
  std::string program = R"(procedure hello {
    while (x > 0) {
      read x;
    }
})";
  REQUIRE(CheckRootIsProgram(program));
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
  REQUIRE(CheckRootIsProgram(program));
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
  REQUIRE(CheckRootIsProgram(program));
};

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
  REQUIRE(CheckRootIsNodeType<ast::NameNode>(input));
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
