#include <assert.h>
#include "sp/ast/and_node.h"
#include "sp/ast/assign_node.h"
#include "sp/ast/ast.h"
#include "sp/ast/conditional_expression_node.h"
#include "sp/ast/constant_node.h"
#include "sp/ast/container_statement_node.h"
#include "sp/ast/divide_node.h"
#include "sp/ast/equals_node.h"
#include "sp/ast/expression_node.h"
#include "sp/ast/factor_node.h"
#include "sp/ast/greater_equals_node.h"
#include "sp/ast/greater_node.h"
#include "sp/ast/identifier_node.h"
#include "sp/ast/if_node.h"
#include "sp/ast/lesser_equals_node.h"
#include "sp/ast/lesser_node.h"
#include "sp/ast/minus_node.h"
#include "sp/ast/modulo_node.h"
#include "sp/ast/name_node.h"
#include "sp/ast/not_node.h"
#include "sp/ast/not_equals_node.h"
#include "sp/ast/or_node.h"
#include "sp/ast/plus_node.h"
#include "sp/ast/print_node.h"
#include "sp/ast/procedure_node.h"
#include "sp/ast/program_node.h"
#include "sp/ast/read_node.h"
#include "sp/ast/relational_expression_node.h"
#include "sp/ast/relational_factor_node.h"
#include "sp/ast/statement_node.h"
#include "sp/ast/statement_list_node.h"
#include "sp/ast/symbol_node.h"
#include "sp/ast/term_node.h"
#include "sp/ast/times_node.h"
#include "sp/ast/variable_node.h"
#include "sp/ast/while_node.h"
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
ast::AST *SimpleParser::Parse(std::vector<token::Token *> input) {
  EndToken *e = new EndToken();
  input.push_back(e);
  stack.empty();
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
  ast::AST *ast;
  if (!stack.size() == 1 || !util::instance_of<ast::ProgramNode>(stack.front())) {
    // Reject condition (guard clause)
    Reject();
    ast = new ast::AST();
    ast->SetRoot(stack.back());
    //assert(false);
    return ast;
  }
  // Success condition
  Success();
  ast = new ast::AST();
  ast->SetRoot(stack.front());
  return ast;
}

void SimpleParser::Shift() {
  // This code is neither DRY nor open for extension
  if (util::instance_of<token::IdentifierToken>(*lookahead)) {
    ast::IdentifierNode *id = new ast::IdentifierNode((*lookahead)->getValue());
    stack.push_back(id);
  } else if (util::instance_of<token::IntegerToken>(*lookahead)) {
    ast::ConstantNode *c = new ast::ConstantNode(std::stoi((*lookahead)->getValue()));
    stack.push_back(c);
  } else if (util::instance_of<token::AndToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kAnd);
    stack.push_back(sym);
  } else if (util::instance_of<token::AssignToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kAssign);
    stack.push_back(sym);
  } else if (util::instance_of<token::DivideToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kDivide);
    stack.push_back(sym);
  } else if (util::instance_of<token::EqualToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::GreaterEqualToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kGreaterEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::GreaterThanToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kGreater);
    stack.push_back(sym);
  } else if (util::instance_of<token::LeftBraceToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kLeftBrace);
    stack.push_back(sym);
  } else if (util::instance_of<token::LeftParenToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kLeftParen);
    stack.push_back(sym);
  } else if (util::instance_of<token::LessEqualToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kLesserEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::LessThanToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kLesser);
    stack.push_back(sym);
  } else if (util::instance_of<token::MinusToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kMinus);
    stack.push_back(sym);
  } else if (util::instance_of<token::ModuloToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kModulo);
    stack.push_back(sym);
  } else if (util::instance_of<token::MultiplyToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kMultiply);
    stack.push_back(sym);
  } else if (util::instance_of<token::NotEqualToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kNotEqual);
    stack.push_back(sym);
  } else if (util::instance_of<token::NotToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kNot);
    stack.push_back(sym);
  } else if (util::instance_of<token::PlusToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kPlus);
    stack.push_back(sym);
  } else if (util::instance_of<token::RightBraceToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kRightBrace);
    stack.push_back(sym);
  } else if (util::instance_of<token::RightParenToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kRightParen);
    stack.push_back(sym);
  } else if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    ast::SymbolNode *sym = new ast::SymbolNode(ast::SymbolType::kSemicolon);
    stack.push_back(sym);
  } else if (util::instance_of<EndToken>(*lookahead)) {
    // end token
    return;
  } else {
    // Default implementation but should not reach here
    ast::IdentifierNode *id = new ast::IdentifierNode((*lookahead)->getValue());
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
  S+ -> Sc S+ <}>
  S+ -> S ; S+ <}>
  S+ -> S ; <}>
  S(r) -> read V <;>
  S(p) -> print V <;>
  S(c) -> call N <;>
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
  L -> E <), >, <, ==, >=, <=, !=>
  L -> V <), >, <, ==, >=, <=, !=>
  L -> C <), >, <, ==, >=, <=, !=>
  R -> L > L <)>
  R -> L < L <)>
  R -> L == L <)>
  R -> L >= L <)>
  R -> L <= L <)>
  R -> L != L <)>
  O -> ! ( O ) <)>
  O -> ( O ) && ( O ) <)>
  O -> ( O ) || ( O ) <)>
  O -> R <)>
  Sc(w) -> while ( O ) { S+ }
  Sc(i) -> if ( O ) then { S+ } else { S+ }
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
      ast::ProgramNode *pr = (ast::ProgramNode *) stack.back();
      stack.pop_back();
      ast::ProcedureNode *p = (ast::ProcedureNode *) stack.back();
      stack.pop_back();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    } else if (util::instance_of<ast::ProcedureNode>(*i)) {
      // Pr <- P
      ast::ProcedureNode *p = (ast::ProcedureNode *) stack.back();
      stack.pop_back();
      ast::ProgramNode *pr = new ast::ProgramNode();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    }
  }
  // procedure
  if (stack.size() >= 5
    && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NameNode>(*std::next(i, 3))
    && util::instance_of<ast::IdentifierNode>(*std::next(i, 4)) && ((ast::IdentifierNode *) *std::next(i, 4))->GetValue() == "procedure") {
    // P <- procedure N { S+ }
    stack.pop_back();
    ast::StatementListNode *sl = (ast::StatementListNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    ast::NameNode *n = (ast::NameNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    ast::ProcedureNode *p = new ast::ProcedureNode(n->GetName(), sl);
    stack.push_back(p);
    return true;
  }
  // statement lists
  if (util::instance_of<token::RightBraceToken>(*lookahead)) {
    // When lookahead is right brace (end of statement list)
    if (stack.size() >= 2
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::ContainerStatementNode>(*std::next(i, 1))) {
      // S+ <- Sc S+
      ast::StatementListNode *sl = (ast::StatementListNode *) stack.back();
      stack.pop_back();
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    }
    if (stack.size() >= 3
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 2))) {
      // S+ <- S ; S+
      ast::StatementListNode *sl = (ast::StatementListNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    } else if (stack.size() >= 2
      && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kSemicolon
      && util::instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // S+ <- S ;
      stack.pop_back();
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      ast::StatementListNode *sl = new ast::StatementListNode();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    }
  }
  // read, print statements
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (stack.size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && ((ast::IdentifierNode *) *std::next(i, 1))->GetValue() == "read") {
      // S(r) <- read V
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::ReadNode *r = new ast::ReadNode(v);
      r->SetStatementNumber(++statementCounter);
      stack.push_back(r);
      return true;
    } else if (stack.size() >= 2
      && util::instance_of<ast::VariableNode>(*i)
      && util::instance_of<ast::IdentifierNode>(*std::next(i, 1)) && ((ast::IdentifierNode *) *std::next(i, 1))->GetValue() == "print") {
      // S(p) <- print V
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::PrintNode *p = new ast::PrintNode(v);
      p->SetStatementNumber(++statementCounter);
      stack.push_back(p);
      return true;
    }
  }
  // factors
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (util::instance_of<ast::VariableNode>(*i)) {
      // F <- V
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      ast::FactorNode *f = new ast::FactorNode(v);
      stack.push_back(f);
      return true;
    } else if (util::instance_of<ast::ConstantNode>(*i)) {
      // F <- C
      ast::ConstantNode *c = (ast::ConstantNode *) stack.back();
      stack.pop_back();
      ast::FactorNode *f = new ast::FactorNode(c);
      stack.push_back(f);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftParen) {
      // F <- ( E )
      stack.pop_back();
      ast::ExpressionNode *e = (ast::ExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::FactorNode *f = new ast::FactorNode(e->GetOperand());
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
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kMultiply
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T * F
      ast::FactorNode *f = (ast::FactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::TermNode *t1 = (ast::TermNode *) stack.back();
      stack.pop_back();
      ast::TimesNode *b = new ast::TimesNode(t1->GetOperand(), f->GetOperand());
      ast::TermNode *t2 = new ast::TermNode(b);
      stack.push_back(t2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kDivide
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      // T <- T / F
      ast::FactorNode *f = (ast::FactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::TermNode *t = (ast::TermNode *) stack.back();
      stack.pop_back();
      ast::DivideNode *b = new ast::DivideNode(t->GetOperand(), f->GetOperand());
      ast::TermNode *t2 = new ast::TermNode(b);
      stack.push_back(t2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::FactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kModulo
      && util::instance_of<ast::TermNode>(*std::next(i, 2))) {
      //T <- T % F
      ast::FactorNode *f = (ast::FactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::TermNode *t = (ast::TermNode *) stack.back();
      stack.pop_back();
      ast::ModuloNode *b = new ast::ModuloNode(t->GetOperand(), f->GetOperand());
      ast::TermNode *t2 = new ast::TermNode(b);
      stack.push_back(t2);
      return true;
    } else if (util::instance_of<ast::FactorNode>(*i)) {
      // T <- F
      ast::FactorNode *f = (ast::FactorNode *) stack.back();
      stack.pop_back();
      ast::TermNode *t = new ast::TermNode(f->GetOperand());
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
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kPlus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E + T < ;, +, ->
      ast::TermNode *t = (ast::TermNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::ExpressionNode *e1 = (ast::ExpressionNode *) stack.back();
      stack.pop_back();
      ast::PlusNode *b = new ast::PlusNode(e1->GetOperand(), t->GetOperand());
      ast::ExpressionNode *e2 = new ast::ExpressionNode(b);
      stack.push_back(e2);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::TermNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kMinus
      && util::instance_of<ast::ExpressionNode>(*std::next(i, 2))) {
      // E <- E - T < ;, +, ->
      ast::TermNode *t = (ast::TermNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::ExpressionNode *e1 = (ast::ExpressionNode *) stack.back();
      stack.pop_back();
      ast::MinusNode *b = new ast::MinusNode(e1->GetOperand(), t->GetOperand());
      ast::ExpressionNode *e2 = new ast::ExpressionNode(b);
      stack.push_back(e2);
      return true;
    } else if (util::instance_of<ast::TermNode>(*i)) {
      // E <- T < ;, +, ->
      ast::TermNode *t = (ast::TermNode *) stack.back();
      stack.pop_back();
      ast::ExpressionNode *e = new ast::ExpressionNode(t->GetOperand());
      stack.push_back(e);
      return true;
    }
  }
  // assign
  if (util::instance_of<token::SemicolonToken>(*lookahead)) {
    if (stack.size() >= 3
      && util::instance_of<ast::ExpressionNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kAssign
      && util::instance_of<ast::VariableNode>(*std::next(i, 2))) {
      // S(a) -> V = E
      ast::ExpressionNode *e = (ast::ExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      ast::AssignNode *a = new ast::AssignNode(v, e);
      a->SetStatementNumber(++statementCounter);
      stack.push_back(a);
      return true;
    }
  }
  // relational factors
  if (util::instance_of<token::RightParenToken>(*lookahead)
    || util::instance_of<token::LessThanToken>(*lookahead)
    || util::instance_of<token::GreaterThanToken>(*lookahead)
    || util::instance_of<token::EqualToken>(*lookahead)
    || util::instance_of<token::LessEqualToken>(*lookahead)
    || util::instance_of<token::GreaterEqualToken>(*lookahead)
    || util::instance_of<token::NotEqualToken>(*lookahead)) {
    if (util::instance_of<ast::ExpressionNode>(*i)) {
      // L <- E
      ast::ExpressionNode *e = (ast::ExpressionNode *) stack.back();
      stack.pop_back();
      ast::RelationalFactorNode *f = new ast::RelationalFactorNode(e->GetOperand());
      stack.push_back(f);
      return true;
    } else if (util::instance_of<ast::VariableNode>(*i)) {
      // L <- V
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      ast::RelationalFactorNode *f = new ast::RelationalFactorNode(v);
      stack.push_back(f);
      return true;
    } else if (util::instance_of<ast::ConstantNode>(*i)) {
      // L <- C
      ast::ConstantNode *c = (ast::ConstantNode *) stack.back();
      stack.pop_back();
      ast::RelationalFactorNode *f = new ast::RelationalFactorNode(c);
      stack.push_back(f);
      return true;
    }
  }
  // relational expression
  if (util::instance_of<token::RightParenToken>(*lookahead)) {
    if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kGreater
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L > L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::GreaterNode *b = new ast::GreaterNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kLesser
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L < L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::LesserNode *b = new ast::LesserNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L == L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::EqualsNode *b = new ast::EqualsNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kGreaterEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L >= L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::GreaterEqualsNode *b = new ast::GreaterEqualsNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kLesserEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L <= L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::LesserEqualsNode *b = new ast::LesserEqualsNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 3
      && util::instance_of<ast::RelationalFactorNode>(*i)
      && util::instance_of<ast::SymbolNode>(*std::next(i, 1)) && ((ast::SymbolNode *) *std::next(i, 1))->GetType() == ast::SymbolType::kNotEqual
      && util::instance_of<ast::RelationalFactorNode>(*std::next(i, 2))) {
      // R <- L == L
      ast::RelationalFactorNode *f1 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::RelationalFactorNode *f2 = (ast::RelationalFactorNode *) stack.back();
      stack.pop_back();
      ast::NotEqualsNode *b = new ast::NotEqualsNode(f2, f1);
      ast::RelationalExpressionNode *e = new ast::RelationalExpressionNode(b);
      stack.push_back(e);
      return true;
    }
  }
  // conditional expressions
  if (util::instance_of<token::RightParenToken>(*lookahead)) {
    if (stack.size() >= 4
      && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *std::next(i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && ((ast::SymbolNode *) *std::next(i, 3))->GetType() == ast::SymbolType::kNot) {
      // O <- ! ( O )
      stack.pop_back();
      ast::ConditionalExpressionNode *n = (ast::ConditionalExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      stack.pop_back();
      ast::NotNode *u = new ast::NotNode(n->GetOperand());
      ast::ConditionalExpressionNode *e = new ast::ConditionalExpressionNode(u);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *std::next(i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && ((ast::SymbolNode *) *std::next(i, 3))->GetType() == ast::SymbolType::kAnd
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && ((ast::SymbolNode *) *std::next(i, 4))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && ((ast::SymbolNode *) *std::next(i, 6))->GetType() == ast::SymbolType::kLeftParen) {
      // O <- ( O ) && ( O )
      stack.pop_back();
      ast::ConditionalExpressionNode *n1 = (ast::ConditionalExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      stack.pop_back();
      stack.pop_back();
      ast::ConditionalExpressionNode *n2 = (ast::ConditionalExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::AndNode *b = new ast::AndNode(n2->GetOperand(), n1->GetOperand());
      ast::ConditionalExpressionNode *e = new ast::ConditionalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 7
      && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *std::next(i))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 1))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftParen
      && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && ((ast::SymbolNode *) *std::next(i, 3))->GetType() == ast::SymbolType::kOr
      && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && ((ast::SymbolNode *) *std::next(i, 4))->GetType() == ast::SymbolType::kRightParen
      && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 5))
      && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && ((ast::SymbolNode *) *std::next(i, 6))->GetType() == ast::SymbolType::kLeftParen) {
      // O <- ( O ) || ( O )
      stack.pop_back();
      ast::ConditionalExpressionNode *n1 = (ast::ConditionalExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      stack.pop_back();
      stack.pop_back();
      ast::ConditionalExpressionNode *n2 = (ast::ConditionalExpressionNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::OrNode *b = new ast::OrNode(n2->GetOperand(), n1->GetOperand());
      ast::ConditionalExpressionNode *e = new ast::ConditionalExpressionNode(b);
      stack.push_back(e);
      return true;
    } else if (stack.size() >= 4
      && util::instance_of<ast::RelationalExpressionNode>(*i)) {
      // O <- R
      ast::RelationalExpressionNode *r = (ast::RelationalExpressionNode *) stack.back();
      stack.pop_back();
      ast::ConditionalExpressionNode *e = new ast::ConditionalExpressionNode(r->GetOperand());
      stack.push_back(e);
      return true;
    }
  }
  // if/while
  if (stack.size() > 7
    && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *std::next(i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::SymbolNode>(*std::next(i, 3)) && ((ast::SymbolNode *) *std::next(i, 3))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 4))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 5)) && ((ast::SymbolNode *) *std::next(i, 5))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NamedNode>(*std::next(i, 6)) && ((ast::NamedNode *) *std::next(i, 6))->GetName() == "while") {
    // Sc(w) <- while ( O ) { S+ }
    // Currently a named node instead of identifier node
    stack.pop_back();
    ast::StatementListNode *s = (ast::StatementListNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    ast::ConditionalExpressionNode *e = (ast::ConditionalExpressionNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    ast::WhileNode *w = new ast::WhileNode(e->GetOperand(), s);
    stack.push_back(w);
  } else if (stack.size() > 12
    && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NamedNode>(*std::next(i, 3)) && ((ast::NamedNode *) *std::next(i, 3))->GetName() == "else"
    && util::instance_of<ast::SymbolNode>(*std::next(i, 4)) && ((ast::SymbolNode *) *std::next(i, 4))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 5))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 6)) && ((ast::SymbolNode *) *std::next(i, 6))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NamedNode>(*std::next(i, 7)) && ((ast::NamedNode *) *std::next(i, 7))->GetName() == "then"
    && util::instance_of<ast::SymbolNode>(*std::next(i, 8)) && ((ast::SymbolNode *) *std::next(i, 8))->GetType() == ast::SymbolType::kRightParen
    && util::instance_of<ast::ConditionalExpressionNode>(*std::next(i, 9))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 10)) && ((ast::SymbolNode *) *std::next(i, 10))->GetType() == ast::SymbolType::kLeftParen
    && util::instance_of<ast::NamedNode>(*std::next(i, 11)) && ((ast::NamedNode *) *std::next(i, 11))->GetName() == "if") {
    // Sc(i) -> if ( O ) then { S+ } else { S+ }
    // Currently a named node instead of identifier node
    stack.pop_back();
    ast::StatementListNode *s1 = (ast::StatementListNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    ast::StatementListNode *s2 = (ast::StatementListNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    ast::ConditionalExpressionNode *e = (ast::ConditionalExpressionNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    stack.pop_back();
    ast::IfNode *w = new ast::IfNode(e->GetOperand(), s2, s1);
    stack.push_back(w);
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
      ast::NameNode *n = (ast::NameNode *) stack.back();
      stack.pop_back();
      ast::VariableNode *v = new ast::VariableNode(n->GetName());
      stack.push_back(v);
      return true;
    }
  }
  // name
  if (!util::instance_of<token::IdentifierToken>(*lookahead)) {
    if (util::instance_of<ast::IdentifierNode>(*i)) {
      // if not identifier then
      // N <- id
      ast::IdentifierNode *id = (ast::IdentifierNode *) stack.back();
      stack.pop_back();
      ast::NameNode *n = new ast::NameNode(id->GetValue());
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
