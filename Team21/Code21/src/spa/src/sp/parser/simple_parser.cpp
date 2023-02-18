#include <assert.h>
#include "sp/ast/assign_node.h"
#include "sp/ast/ast.h"
#include "sp/ast/constant_node.h"
#include "sp/ast/divide_node.h"
#include "sp/ast/expression_node.h"
#include "sp/ast/factor_node.h"
#include "sp/ast/identifier_node.h"
#include "sp/ast/minus_node.h"
#include "sp/ast/modulo_node.h"
#include "sp/ast/name_node.h"
#include "sp/ast/plus_node.h"
#include "sp/ast/print_node.h"
#include "sp/ast/procedure_node.h"
#include "sp/ast/program_node.h"
#include "sp/ast/read_node.h"
#include "sp/ast/statement_node.h"
#include "sp/ast/statement_list_node.h"
#include "sp/ast/symbol_node.h"
#include "sp/ast/term_node.h"
#include "sp/ast/times_node.h"
#include "sp/ast/variable_node.h"
#include "token/and_token.h"
#include "token/assign_token.h"
#include "token/divide_token.h"
#include "token/equal_token.h"
#include "token/greater_equal_token.h"
#include "token/greater_than_token.h"
#include "token/identifier_token.h"
#include "token/integer_token.h"
#include "token/left_brace_token.h"
#include "token/left_paren_token.h"
#include "token/less_equal_token.h"
#include "token/less_than_token.h"
#include "token/minus_token.h"
#include "token/modulo_token.h"
#include "token/multiply_token.h"
#include "token/not_equal_token.h"
#include "token/not_token.h"
#include "token/or_token.h"
#include "token/plus_token.h"
#include "token/right_brace_token.h"
#include "token/right_paren_token.h"
#include "token/semicolon_token.h"
#include "simple_parser.h"
#include "util/instance_of.h"

using namespace util;

namespace parser {
std::unique_ptr<ast::AST> SimpleParser::Parse(std::vector<std::unique_ptr<token::Token>> input) {
  std::unique_ptr<token::Token> e = std::make_unique<EndToken>();
  input.push_back(e);
  (void) stack.empty();
  statementCounter = 0;
  for (lookahead = input.begin(); lookahead < input.end(); lookahead++) {
    // If empty stack, can just shift
    if (stack.size() == 0) {
      Shift();
      continue;
    }

    while (Check()) {

    }

    Shift();
  }
  std::unique_ptr<ast::AST> ast;
  if (!stack.size() == 1 || !util::instance_of<ast::ProgramNode>(stack.front())) {
    // Reject condition (guard clause)
    Reject();
    ast = std::make_unique<ast::AST>();
    ast->SetRoot(stack.back());
    //assert(false);
    return ast;
  }
  // Success condition
  Success();
  ast = std::make_unique<ast::AST>();
  ast->SetRoot(stack.front());
  return ast;
}

void SimpleParser::Shift() {
  // This code is neither DRY nor open for extension
  if (util::instance_of<token::IdentifierToken>(*lookahead)) {
    std::shared_ptr<ast::IdentifierNode> id = std::make_shared<ast::IdentifierNode>((*lookahead)->getValue());
    stack.push_back(id);
  } else if (util::instance_of<token::IntegerToken>(*lookahead)) {
    std::shared_ptr<ast::ConstantNode> c = std::make_shared<ast::ConstantNode>(std::stoi((*lookahead)->getValue()));
    stack.push_back(c);
  } else if (util::instance_of<token::AndToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAnd);
    stack.push_back(sym);
  } else if (util::instance_of<token::AssignToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAssign);
    stack.push_back(sym);
  } else if (util::instance_of<token::DivideToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kDivide);
    stack.push_back(sym);
  } else if (util::instance_of<token::EqualToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::GreaterEqualToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreaterEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::GreaterThanToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreater);
    stack.push_back(sym);
  } else if (util::instance_of<token::LeftBraceToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftBrace);
    stack.push_back(sym);
  } else if (util::instance_of<token::LeftParenToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftParen);
    stack.push_back(sym);
  } else if (util::instance_of<token::LessEqualToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesserEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::LessThanToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesser);
    stack.push_back(sym);
  } else if (util::instance_of<token::MinusToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMinus);
    stack.push_back(sym);
  } else if (util::instance_of<token::ModuloToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kModulo);
    stack.push_back(sym);
  } else if (util::instance_of<token::MultiplyToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMultiply);
    stack.push_back(sym);
  } else if (util::instance_of<token::NotEqualToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNotEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::NotToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNot);
    stack.push_back(sym);
  } else if (util::instance_of<token::PlusToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kPlus);
    stack.push_back(sym);
  } else if (util::instance_of<token::RightBraceToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightBrace);
    stack.push_back(sym);
  } else if (util::instance_of<token::RightParenToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightParen);
    stack.push_back(sym);
  } else if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kSemicolon);
    stack.push_back(sym);
  } else if (util::instance_of<EndToken>(*lookahead)) {
    // end token
    return;
  } else {
    // Default implementation but should not reach here
    std::shared_ptr<ast::IdentifierNode> id = std::make_shared<ast::IdentifierNode>((*lookahead)->getValue());
    stack.push_back(id);
    assert(false);
  }
}

void SimpleParser::Reduce() {

}

void SimpleParser::Reject() {

}

void SimpleParser::Success() {

}

/*
  Grammar rules:
  Pr -> P Pr <$>
  Pr -> P <$>
  P -> procedure N { S+ }
  S+ -> S ; S+ <}>
  S+ -> S ; <}>
  S(r) -> read V <;>
  S(p) -> print V <;>
  F -> V <;>
  F -> C <;>
  F -> ( E ) <;>
  T -> T * F <;, +, -, *, /, %>
  T -> T / F <;, +, -, *, /, %>
  T -> T % F <;, +, -, *, /, %>
  T -> F <;, +, -, *, /, %>
  E -> E + T <;, +, ->
  E -> E - T <;, +, ->
  E -> T <;, +, ->
  S(a) -> V = E <;>
  V -> N <=, ;, +, -, *, /, %, )>
  N -> id <!id>
  C -> int
*/
bool SimpleParser::Check() {
  auto i = stack.rbegin();
  // program
  if (util::instance_of<EndToken>(*lookahead)) {
    // When token has reached end
    if (stack.size() >= 2
      && util::instance_of<ast::ProgramNode>(*i)
      && util::instance_of<ast::ProcedureNode>(*std::next(i, 1))) {
      // TODO: Fix if statement formatting
      // Pr <- P Pr
      std::shared_ptr<ast::ProgramNode> pr = std::static_pointer_cast<ast::ProgramNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::ProcedureNode> p = std::static_pointer_cast<ast::ProcedureNode>(stack.back());
      stack.pop_back();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    } else if (util::instance_of<ast::ProcedureNode>(*i)) {
      // Pr <- P
      std::shared_ptr<ast::ProcedureNode> p = std::static_pointer_cast<ast::ProcedureNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::ProgramNode> pr = std::make_shared<ast::ProgramNode>();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    }
  }
  // procedure
  if (stack.size() >= 5
    && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NameNode>(*std::next(i, 3))
    && util::instance_of<ast::IdentifierNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 4)))->GetValue() == "procedure") {
    // P <- procedure N { S+ }
    stack.pop_back();
    std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack.back());
    stack.pop_back();
    stack.pop_back();
    std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack.back());
    stack.pop_back();
    stack.pop_back();
    std::shared_ptr<ast::ProcedureNode> p = std::make_shared<ast::ProcedureNode>(n->GetName(), sl);
    stack.push_back(p);
    return true;
  }
  // statement lists
  if (util::instance_of<token::RightBraceToken>(*lookahead)) {
    // When lookahead is right brace (end of statement list)
    if (stack.size() >= 3
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 2))) {
      // S+ <- S ; S+
      std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack.back());
      stack.pop_back();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    } else if (stack.size() >= 2
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // S+ <- S ;
      stack.pop_back();
      std::shared_ptr<ast::StatementNode> s = std::static_pointer_cast<ast::StatementNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::StatementListNode> sl = std::make_shared<ast::StatementListNode>();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    }
  }
  // read, print statements
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (stack.size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "read") {
      // S(r) <- read V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::ReadNode> r = std::make_shared<ast::ReadNode>(v);
      r->SetStatementNumber(++statementCounter);
      stack.push_back(r);
      return true;
    } else if (stack.size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 1)))->GetValue() == "print") {
      // S(p) <- print V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::PrintNode> p = std::make_shared<ast::PrintNode>(v);
      p->SetStatementNumber(++statementCounter);
      stack.push_back(p);
      return true;
    }
  }
  // factors
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (util::instance_of<ast::VariableNode>(*i)) {
      // F <- V
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(v);
      stack.push_back(f);
      return true;
    } else if (util::instance_of<ast::ConstantNode>(*i)) {
      // F <- C
      std::shared_ptr<ast::ConstantNode> c = std::static_pointer_cast<ast::ConstantNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(c);
      stack.push_back(f);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftParen) {
      // F <- ( E )
      stack.pop_back();
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::FactorNode> f = std::make_shared<ast::FactorNode>(e->GetOperand());
      stack.push_back(f);
      return true;
    }
  }
  // terms
  if (util::instance_of<token::SemicolonToken>(*lookahead)
    || util::instance_of<token::PlusToken>(*lookahead)
    || util::instance_of<token::MinusToken>(*lookahead)
    || util::instance_of<token::MultiplyToken>(*lookahead)
    || util::instance_of<token::DivideToken>(*lookahead)
    || util::instance_of<token::ModuloToken>(*lookahead)) {
    if (stack.size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMultiply
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T * F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::TermNode> t1 = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::TimesNode> b = std::make_shared<ast::TimesNode>(t1->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack.push_back(t2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kDivide
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T / F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::DivideNode> b = std::make_shared<ast::DivideNode>(t->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack.push_back(t2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kModulo
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      //T <- T % F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::ModuloNode> b = std::make_shared<ast::ModuloNode>(t->GetOperand(), f->GetOperand());
      std::shared_ptr<ast::TermNode> t2 = std::make_shared<ast::TermNode>(b);
      stack.push_back(t2);
      return true;
    } else if (util::instance_of<ast::FactorNode>(*i)) {
      // T <- F
      std::shared_ptr<ast::FactorNode> f = std::static_pointer_cast<ast::FactorNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::TermNode> t = std::make_shared<ast::TermNode>(f->GetOperand());
      stack.push_back(t);
      return true;
    }
  }
  // expressions
  if (util::instance_of<token::SemicolonToken>(*lookahead)
    || util::instance_of<token::PlusToken>(*lookahead)
    || util::instance_of<token::MinusToken>(*lookahead)) {
    if (stack.size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kPlus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E + T < ;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::PlusNode> b = std::make_shared<ast::PlusNode>(e1->GetOperand(), t->GetOperand());
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      stack.push_back(e2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kMinus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E - T < ;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::ExpressionNode> e1 = std::static_pointer_cast<ast::ExpressionNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::MinusNode> b = std::make_shared<ast::MinusNode>(e1->GetOperand(), t->GetOperand());
      std::shared_ptr<ast::ExpressionNode> e2 = std::make_shared<ast::ExpressionNode>(b);
      stack.push_back(e2);
      return true;
    } else if (util::instance_of<ast::TermNode>(*i)) {
      // E <- T < ;, +, ->
      std::shared_ptr<ast::TermNode> t = std::static_pointer_cast<ast::TermNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::ExpressionNode> e = std::make_shared<ast::ExpressionNode>(t->GetOperand());
      stack.push_back(e);
      return true;
    }
  }
  // assign
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (stack.size() >= 3
      && util::instance_of<ast::ExpressionNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 1)))->GetType() == ast::SymbolType::kEqual
      && util::instance_of<ast::VariableNode>(*std::next(i, 2))) {
      // S(a) -> V = E
      std::shared_ptr<ast::ExpressionNode> e = std::static_pointer_cast<ast::ExpressionNode>(stack.back());
      stack.pop_back();
      stack.pop_back();
      std::shared_ptr<ast::VariableNode> v = std::static_pointer_cast<ast::VariableNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::AssignNode> a = std::make_shared<ast::AssignNode>(v, e);
      a->SetStatementNumber(++statementCounter);
      stack.push_back(a);
      return true;
    }
  }
  // variable
  if (util::instance_of<token::EqualToken>(*lookahead)
    || util::instance_of<token::SemicolonToken>(*lookahead)
    || util::instance_of<token::PlusToken>(*lookahead)
    || util::instance_of<token::MinusToken>(*lookahead)
    || util::instance_of<token::MultiplyToken>(*lookahead)
    || util::instance_of<token::DivideToken>(*lookahead)
    || util::instance_of<token::ModuloToken>(*lookahead)
    || util::instance_of<token::RightParenToken>(*lookahead)) {
    if (util::instance_of<ast::NameNode>(*i)) {
      // V <- N
      std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::VariableNode> v = std::make_shared<ast::VariableNode>(n->GetName());
      stack.push_back(v);
      return true;
    }
  }
  // name
  if (!util::instance_of<token::IdentifierToken>(*lookahead)) {
    if (util::instance_of<ast::IdentifierNode>(*i)) {
      // if not identifier then
      // N <- id
      std::shared_ptr<ast::IdentifierNode> id = std::static_pointer_cast<ast::IdentifierNode>(stack.back());
      stack.pop_back();
      std::shared_ptr<ast::NameNode> n = std::make_shared<ast::NameNode>(id->GetValue());
      stack.push_back(n);
      return true;
    }
  }
  return false;
}

const std::string EndToken::getValue() {
  return "$";
}
}
