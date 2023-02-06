#include "sp/ast/ast.h"
#include "sp/ast/identifier_node.h"
#include "sp/ast/name_node.h"
#include "sp/ast/procedure_node.h"
#include "sp/ast/program_node.h"
#include "sp/ast/read_node.h"
#include "sp/ast/statement_node.h"
#include "sp/ast/statement_list_node.h"
#include "sp/ast/symbol_node.h"
#include "sp/ast/variable_node.h"
#include "token/equal_token.h"
#include "token/identifier_token.h"
#include "token/right_brace_token.h"
#include "simple_parser.h"
#include "util/instance_of.h"

using namespace util;

namespace parser {
ast::AST *SimpleParser::Parse(std::vector<token::Token *> input) {
  EndToken *e = new EndToken();
  input.push_back(e);
  stack.empty();
  for (lookahead = input.begin(); lookahead < input.end(); lookahead++) {
    // If empty stack, can just shift
    if (stack.size() == 0) {
      Shift();
      continue;
    }

    while (Check()) {

    }
  }
  return new ast::AST();
}

void SimpleParser::Shift() {

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
  V -> N <=>
  N -> id <!id>
*/
bool SimpleParser::Check() {
  auto i = stack.rbegin();
  if (instance_of<EndToken>(*lookahead)) {
    // When token has reached end
    if (instance_of<ast::ProgramNode>(*i)
        && instance_of<ast::ProcedureNode>(*std::next(i, 1))) {
      // Pr <- P Pr
      ast::ProgramNode *pr = (ast::ProgramNode *) stack.back();
      stack.pop_back();
      ast::ProcedureNode *p = (ast::ProcedureNode *) stack.back();
      stack.pop_back();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    } else if (instance_of<ast::ProcedureNode>(*i)) {
      // Pr <- P;
      ast::ProcedureNode *p = (ast::ProcedureNode *) stack.back();
      stack.pop_back();
      ast::ProgramNode *pr = new ast::ProgramNode();
      pr->AddProcedure(p);
      stack.push_back(pr);
      return true;
    }
  } else if (instance_of<token::RightBraceToken>(*lookahead)) {
    // When lookahead is right brace (end of statement list)
    if (instance_of<ast::StatementListNode>(*i)
        && instance_of<ast::StatementNode>(*std::next(i, 1))) {
      // S+ <- S S+
      ast::StatementListNode *sl = (ast::StatementListNode *) stack.back();
      stack.pop_back();
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    } else if (instance_of<ast::StatementNode>(*i)) {
      // S+ <- S
      ast::StatementNode *s = (ast::StatementNode *) stack.back();
      stack.pop_back();
      ast::StatementListNode *sl = new ast::StatementListNode();
      sl->AddStatement(s);
      stack.push_back(sl);
      return true;
    }
  } else if (instance_of<token::EqualToken>(*lookahead)) {
    // assignment (not yet added other ops)
    if (instance_of<ast::NameNode>(*i)) {
      // V <- N
      ast::NameNode *n = (ast::NameNode *) stack.back();
      stack.pop_back();
      ast::VariableNode *v = new ast::VariableNode(n->GetName());
      stack.push_back(v);
      return true;
    }
  } else {
    if (instance_of<ast::SymbolNode>(*i) && ((ast::SymbolNode *) *i)->GetType() == ast::SymbolType::kRightBrace
        && instance_of<ast::StatementListNode>(*std::next(i, 1))
        && instance_of<ast::SymbolNode>(*std::next(i, 2)) && ((ast::SymbolNode *) *std::next(i, 2))->GetType() == ast::SymbolType::kLeftBrace
        && instance_of<ast::NameNode>(*std::next(i, 3))
        && instance_of<ast::IdentifierNode>(*std::next(i, 4)) && ((ast::IdentifierNode *) *std::next(i, 4))->GetValue() == "procedure") {
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
    } else if (instance_of<ast::VariableNode>(*i)
               && instance_of<ast::IdentifierNode>(*std::next(i, 1)) && ((ast::IdentifierNode *) *std::next(i, 1))->GetValue() == "read") {
      // S(r) <- read V;
      ast::VariableNode *v = (ast::VariableNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ast::ReadNode *r = new ast::ReadNode(v);
      stack.push_back(r);
      return true;
    } else if (!instance_of<token::IdentifierToken>(*lookahead)
               && instance_of<ast::IdentifierNode>(*i)) {
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
