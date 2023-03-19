#include "simple_chain_parser.h"

#include <cassert>

#include "exceptions/syntax_error.h"
#include "subparsers.h"
#include "sp/token/and_token.h"
#include "sp/token/assign_token.h"
#include "sp/token/divide_token.h"
#include "sp/token/end_token.h"
#include "sp/token/equal_token.h"
#include "sp/token/greater_equal_token.h"
#include "sp/token/greater_than_token.h"
#include "sp/token/identifier_token.h"
#include "sp/token/integer_token.h"
#include "sp/token/left_brace_token.h"
#include "sp/token/left_paren_token.h"
#include "sp/token/less_equal_token.h"
#include "sp/token/less_than_token.h"
#include "sp/token/minus_token.h"
#include "sp/token/modulo_token.h"
#include "sp/token/multiply_token.h"
#include "sp/token/not_equal_token.h"
#include "sp/token/not_token.h"
#include "sp/token/or_token.h"
#include "sp/token/plus_token.h"
#include "sp/token/right_brace_token.h"
#include "sp/token/right_paren_token.h"
#include "sp/token/semicolon_token.h"
#include "util/instance_of.h"

namespace parser {
SimpleChainParser::SimpleChainParser() noexcept {
  this->subparsers = std::make_shared<ProgramSubparser>();
  this->subparsers->SetNext(std::make_shared<ProcedureSubparser>())
    ->SetNext(std::make_shared<StatementListSubparser>())
    ->SetNext(std::make_shared<ReadSubparser>())
    ->SetNext(std::make_shared<PrintSubparser>())
    ->SetNext(std::make_shared<CallSubparser>())
    ->SetNext(std::make_shared<FactorSubparser>())
    ->SetNext(std::make_shared<TermSubparser>())
    ->SetNext(std::make_shared<ExpressionSubparser>())
    ->SetNext(std::make_shared<AssignSubparser>())
    ->SetNext(std::make_shared<RelationalFactorSubparser>())
    ->SetNext(std::make_shared<RelationalExpressionSubparser>())
    ->SetNext(std::make_shared<ConditionalExpressionSubparser>())
    ->SetNext(std::make_shared<IfSubparser>())
    ->SetNext(std::make_shared<WhileSubparser>())
    ->SetNext(std::make_shared<VariableSubparser>())
    ->SetNext(std::make_shared<NameSubparser>());
}

auto SimpleChainParser::Parse(std::vector<std::unique_ptr<token::Token>> input) -> std::unique_ptr<ast::AST> {
  // Push end token to mark end of token list
  input.push_back(std::make_unique<token::EndToken>());
  auto lookahead = std::make_shared<std::vector<std::unique_ptr<token::Token>>::iterator>(input.begin());
  context = std::make_shared<Context>(lookahead);
  context->GetStack()->clear();
  context->GetStatementCounter() = 0;
  for (*lookahead; *lookahead < input.end(); (*lookahead)++) {
    // Handle parsing logic
    if (context->GetStack()->empty()) {
      Shift();
      continue;
    }

    while (subparsers->Parse(context)) {}

    Shift();
  }
  if (context->GetStack()->empty()) {
    throw exceptions::SyntaxError("Empty program");
  }
  if (context->GetStack()->size() != 1 || !util::instance_of<ast::ProgramNode>(context->GetStack()->front())) {
    // Reject condition (guard clause)
  }
  std::unique_ptr<ast::AST> ast = std::make_unique<ast::AST>();
  ast->SetRoot(context->GetStack()->front());
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
  if (util::instance_of<token::IdentifierToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::IdentifierNode> ide = std::make_shared<ast::IdentifierNode>((**(context->GetLookahead()))->GetValue());
    context->GetStack()->push_back(ide);
  } else if (util::instance_of<token::IntegerToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::ConstantNode> con = std::make_shared<ast::ConstantNode>(std::stoi((**(context->GetLookahead()))->GetValue()));
    context->GetStack()->push_back(con);
  } else if (util::instance_of<token::AndToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAnd);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::AssignToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kAssign);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::DivideToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kDivide);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::EqualToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kEqual);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::GreaterEqualToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreaterEqual);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::GreaterThanToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kGreater);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::LeftBraceToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftBrace);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::LeftParenToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLeftParen);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::LessEqualToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesserEqual);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::LessThanToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kLesser);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::MinusToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMinus);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::ModuloToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kModulo);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::MultiplyToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kMultiply);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::NotEqualToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNotEqual);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::NotToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kNot);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::OrToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kOr);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::PlusToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kPlus);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::RightBraceToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightBrace);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::RightParenToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kRightParen);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::SemicolonToken>(**(context->GetLookahead()))) {
    std::shared_ptr<ast::SymbolNode> sym = std::make_shared<ast::SymbolNode>(ast::SymbolType::kSemicolon);
    context->GetStack()->push_back(sym);
  } else if (util::instance_of<token::EndToken>(**(context->GetLookahead()))) {
    // No need to include end token in AST
    return;
  } else {
    // Default implementation but should not reach here
    std::shared_ptr<ast::IdentifierNode> ide = std::make_shared<ast::IdentifierNode>((**(context->GetLookahead()))->GetValue());
    context->GetStack()->push_back(ide);
    assert(false); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  }
}
}  // namespace parser
