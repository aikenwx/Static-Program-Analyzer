#include "program_subparser.h"
#include "token/and_token.h"
#include "token/assign_token.h"
#include "token/divide_token.h"
#include "token/end_token.h"
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
#include "simple_chain_parser.h"
#include "util/instance_of.h"

namespace parser {
SimpleChainParser::SimpleChainParser() {
  this->subparsers = std::make_shared<ProgramSubparser>();
}

std::unique_ptr<ast::AST> SimpleChainParser::Parse(std::vector<std::unique_ptr<token::Token>> input) {
  // Push end token to mark end of token list
  input.push_back(std::make_unique<token::EndToken>());
  context = std::make_shared<Context>();
  context->stack.clear();
  context->statementCounter = 0;
  for (context->lookahead = input.begin(); context->lookahead < input.end(); context->lookahead++) {
    // Handle parsing logic
    while (subparsers->Parse(context));

  }
  ast = std::make_unique<ast::AST>();
  return ast;
}

/*
* Shifts a token into the parse stack and converts it to a node object.
*/
void SimpleChainParser::Shift() {
  /*
  * If statements trade open-closed for single-responsibility and interface segregation
  * between tokenizer and parser. Since grammar is fixed, open-closed not as necessary.
  */
  if (util::instance_of<token::IdentifierToken>(context->*lookahead)) {
    std::shared_ptr<ast::IdentifierNode> id = std::make_shared<ast::IdentifierNode>((context->*lookahead)->GetValue());
    context->stack.push_back(id);
  } else if (util::instance_of<token::IntegerToken>(context->*lookahead)) {
    std::shared_ptr<ast::ConstantNode> c = std::make_shared<ast::ConstantNode>(std::stoi((context->*lookahead)->GetValue()));
    context->stack.push_back(c);
  } else if (util::instance_of<token::AndToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAnd);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::AssignToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAssign);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::DivideToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kDivide);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::EqualToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kEqual);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::GreaterEqualToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreaterEqual);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::GreaterThanToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreater);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::LeftBraceToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftBrace);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::LeftParenToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftParen);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::LessEqualToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesserEqual);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::LessThanToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesser);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::MinusToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMinus);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::ModuloToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kModulo);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::MultiplyToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMultiply);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::NotEqualToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNotEqual);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::NotToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNot);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::OrToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kOr);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::PlusToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kPlus);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::RightBraceToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightBrace);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::RightParenToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightParen);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::SemicolonToken>(context->*lookahead)) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kSemicolon);
    context->stack.push_back(sym);
  } else if (util::instance_of<token::EndToken>(context->*lookahead)) {
    // No need to include end token in AST
    return;
  } else {
    // Default implementation but should not reach here
    std::shared_ptr<ast::IdentifierNode> id = std::make_shared<ast::IdentifierNode>((*lookahead)->GetValue());
    context->stack.push_back(id);
    assert(false);
  }
}
}