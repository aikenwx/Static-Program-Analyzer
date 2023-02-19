#include <assert.h>
#include "sp/ast/ast.h"
#include "sp/ast/identifier_node.h"
#include "sp/ast/name_node.h"
#include "sp/ast/print_node.h"
#include "sp/ast/procedure_node.h"
#include "sp/ast/program_node.h"
#include "sp/ast/read_node.h"
#include "sp/ast/statement_node.h"
#include "sp/ast/statement_list_node.h"
#include "sp/ast/symbol_node.h"
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
    ast::IdentifierNode *id = new ast::IdentifierNode((*lookahead)->GetValue());
    stack.push_back(id);
  } else if (util::instance_of<token::IntegerToken>(*lookahead)) {
    // Unimplemented
    assert(false);
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
    ast::IdentifierNode *id = new ast::IdentifierNode((*lookahead)->GetValue());
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
  S+ -> S S+ <}>
  S+ -> S <}>
  S(r) -> read V ;
  S(p) -> print V ;
  V -> N <=, ;>
  N -> id <!id>
*/
bool SimpleParser::Check() {
  auto i = stack.rbegin();
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
  } else if (util::instance_of<token::RightBraceToken>(*lookahead)) {
    // When lookahead is right brace (end of statement list)
    if (stack.size() >= 2
      && util::instance_of<ast::StatementListNode>(*i)
      && util::instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // S+ <- S S+
      ast::StatementListNode *sl = (ast::StatementListNode *) stack.back();
      stack.pop_back();
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    } else if (util::instance_of<ast::StatementNode>(*i)) {
      // S+ <- S
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      ast::StatementListNode *sl = new ast::StatementListNode();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    }
  } else if (util::instance_of<token::EqualToken>(*lookahead)
    || util::instance_of<token::SemicolonToken>(*lookahead)) {
    // assignment (not yet added other ops)
    if (util::instance_of<ast::NameNode>(*i)) {
      // V <- N
      ast::NameNode *n = (ast::NameNode *) stack.back();
      stack.pop_back();
      ast::VariableNode *v = new ast::VariableNode(n->GetName());
      stack.push_back(v);
      return true;
    }
  }
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
  } else if (stack.size() >= 3
    && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kSemicolon
    && util::instance_of<ast::VariableNode>(*std::next(i, 1))
    && util::instance_of<ast::IdentifierNode>(*std::next(i, 2)) && ((ast::IdentifierNode *) *std::next(i, 2))->GetValue() == "read") {
    // S(r) <- read V ;
    stack.pop_back();
    ast::VariableNode *v = (ast::VariableNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    ast::ReadNode *r = new ast::ReadNode(v);
    r->SetStatementNumber(++statementCounter);
    stack.push_back(r);
    return true;
  } else if (stack.size() >= 3
    && util::instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kSemicolon
    && util::instance_of<ast::VariableNode>(*std::next(i, 1))
    && util::instance_of<ast::IdentifierNode>(*std::next(i, 2)) && ((ast::IdentifierNode *) *std::next(i, 2))->GetValue() == "print") {
    // S(p) <- print V ;
    stack.pop_back();
    ast::VariableNode *v = (ast::VariableNode *) stack.back();
    stack.pop_back();
    stack.pop_back();
    ast::PrintNode *p = new ast::PrintNode(v);
    p->SetStatementNumber(++statementCounter);
    stack.push_back(p);
    return true;
  } else if (!util::instance_of<token::IdentifierToken>(*lookahead)
    && util::instance_of<ast::IdentifierNode>(*i)) {
    // if not identifier then
    // N <- id
    ast::IdentifierNode *id = (ast::IdentifierNode *) stack.back();
    stack.pop_back();
    ast::NameNode *n = new ast::NameNode(id->GetValue());
    stack.push_back(n);
    return true;
  }
  return false;
}
}
