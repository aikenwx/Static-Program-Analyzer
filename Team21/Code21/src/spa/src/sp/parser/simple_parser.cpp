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

AST *SimpleParser::parse(std::vector<token::Token *> input) {
  EndToken *e = new EndToken();
  input.push_back(e);
  stack.empty();
  for (lookahead = input.begin(); lookahead < input.end(); lookahead++) {
    // If empty stack, can just shift
    if (stack.size() == 0) {
      shift();
      continue;
    }
    
    while (check()) {

    }
  }
  return &AST();
}

void SimpleParser::shift() {

}

void SimpleParser::reduce() {

}

void SimpleParser::reject() {

}

void SimpleParser::success() {

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
bool SimpleParser::check() {
  auto i = stack.rbegin();
  if (instance_of<EndToken>(*lookahead)) {
    // When token has reached end
    if (instance_of<ProgramNode>(*i)
        && instance_of<ProcedureNode>(*std::next(i, 1))) {
      // Pr <- P Pr
      ProgramNode *pr = (ProgramNode *) stack.back();
      stack.pop_back();
      ProcedureNode *p = (ProcedureNode *) stack.back();
      stack.pop_back();
      pr->addProcedure(p);
      stack.push_back(pr);
      return true;
    } else if (instance_of<ProcedureNode>(*i)) {
      // Pr <- P;
      ProcedureNode *p = (ProcedureNode *) stack.back();
      stack.pop_back();
      ProgramNode *pr = &ProgramNode();
      pr->addProcedure(p);
      stack.push_back(pr);
      return true;
    }
  } else if (instance_of<token::RightBraceToken>(*lookahead)) {
    // When lookahead is right brace (end of statement list)
    if (instance_of<StatementListNode>(*i)
        && instance_of<StatementNode>(*std::next(i, 1))) {
      // S+ <- S S+
      StatementListNode *sl = (StatementListNode *) stack.back();
      stack.pop_back();
      StatementNode *s = (StatementNode *) stack.back();
      stack.pop_back();
      sl->addStatement(s);
      stack.push_back(sl);
      return true;
    } else if (instance_of<StatementNode>(*i)) {
      // S+ <- S
      StatementNode *s = (StatementNode *) stack.back();
      stack.pop_back();
      StatementListNode *sl = &StatementListNode();
      sl->addStatement(s);
      stack.push_back(sl);
      return true;
    }
  } else if (instance_of<token::EqualToken>(*lookahead)) {
    // assignment (not yet added other ops)
    if (instance_of<NameNode>(*i)) {
      // V <- N
      NameNode *n = (NameNode *) stack.back();
      stack.pop_back();
      VariableNode *v = &VariableNode(n->getName());
      stack.push_back(v);
      return true;
    }
  } else {
    if (instance_of<SymbolNode>(*i) && ((SymbolNode *) *i)->getType() == SymbolType::kRightBrace
        && instance_of<StatementListNode>(*std::next(i, 1))
        && instance_of<SymbolNode>(*std::next(i, 2)) && ((SymbolNode *) *std::next(i, 2))->getType() == SymbolType::kLeftBrace
        && instance_of<NameNode>(*std::next(i, 3))
        && instance_of<IdentifierNode>(*std::next(i, 4)) && ((IdentifierNode *) *std::next(i, 4))->getValue() == "procedure") {
      // P <- procedure N { S+ }
      stack.pop_back();
      StatementListNode *sl = (StatementListNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      NameNode *n = (NameNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ProcedureNode *p = &ProcedureNode(n->getName(), sl);
      stack.push_back(p);
      return true;
    } else if (instance_of<VariableNode>(*i)
               && instance_of<IdentifierNode>(*std::next(i, 1)) && ((IdentifierNode *) *std::next(i, 1))->getValue() == "read") {
      // S(r) <- read V;
      VariableNode *v = (VariableNode *) stack.back();
      stack.pop_back();
      stack.pop_back();
      ReadNode *r = &ReadNode(v);
      stack.push_back(r);
      return true;
    } else if (!instance_of<token::IdentifierToken>(*lookahead)
               && instance_of<IdentifierNode>(*i)) {
      // if not identifier then
      // N <- id
      IdentifierNode *id = (IdentifierNode *) stack.back();
      stack.pop_back();
      NameNode *n = &NameNode(id->getValue());
      stack.push_back(n);
      return true;
    }
  }
  return false;
}

const std::string EndToken::getValue() {
  return "$";
}
