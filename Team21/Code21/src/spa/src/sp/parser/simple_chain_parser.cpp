#include "simple_chain_parser.h"

#include <algorithm>
#include <cassert>

#include "exceptions/syntax_error.h"
#include "sp/ast/symbol_node.h"
#include "sp/token/end_token.h"
#include "sp/token/identifier_token.h"
#include "sp/token/integer_token.h"
#include "sp/token/symbol_token.h"
#include "sp/token/token.h"
#include "subparsers.h"
#include "util/instance_of.h"

namespace parser {
SimpleChainParser::SimpleChainParser() noexcept {
  this->subparsers = std::make_shared<ProgramSubparser>();
  this->subparsers->SetNext(std::make_shared<ProcedureSubparser>())
      ->SetNext(std::make_shared<StatementListSubparser>())
      ->SetNext(std::make_shared<ReadSubparser>())
      ->SetNext(std::make_shared<PrintSubparser>())
      ->SetNext(std::make_shared<CallSubparser>())
      ->SetNext(std::make_shared<TermSubparser>())
      ->SetNext(std::make_shared<ExpressionSubparser>())
      ->SetNext(std::make_shared<AssignSubparser>())
      ->SetNext(std::make_shared<ConditionalExpressionSubparser>())
      ->SetNext(std::make_shared<IfSubparser>())
      ->SetNext(std::make_shared<WhileSubparser>());
}

auto SimpleChainParser::Parse(std::vector<std::unique_ptr<token::Token>> input)
    -> std::pair<bool, std::unique_ptr<ast::AST>> {
  // Push end token to mark end of token list
  input.push_back(token::EndToken::CreateToken());
  auto lookahead =
      std::make_shared<std::vector<std::unique_ptr<token::Token>>::iterator>(
          input.begin());
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
  auto ast = std::make_unique<ast::AST>();
  if (context->GetStack()->size() != 1 ||
      !util::instance_of<ast::ProgramNode>(context->GetStack()->front())) {
    ast->SetRoot(context->GetStack()->front());
    return std::make_pair(false, std::move(ast));
  }
  ast->SetRoot(context->GetStack()->front());
  return std::make_pair(true, std::move(ast));;
}

/*
 * Shifts a token into the parse stack and converts it to a node object.
 */
void SimpleChainParser::Shift() {
  /*
   * If statements trade open-closed for single-responsibility and interface
   * segregation between tokenizer and parser. Since grammar is fixed,
   * open-closed not as necessary.
   */
  if (context->IsLookaheadTypeOf<token::IdentifierToken>()) {
    auto ide = std::make_shared<ast::IdentifierNode>(
        (**(context->GetLookahead()))->GetValue());
    context->GetStack()->push_back(ide);
  } else if (context->IsLookaheadTypeOf<token::IntegerToken>()) {
    auto con = std::make_shared<ast::ConstantNode>(
        std::stoi((**(context->GetLookahead()))->GetValue()));
    context->GetStack()->push_back(con);
  } else if (context->IsLookaheadTypeOf<token::SymbolToken>()) {
    auto symbol_type =
        kSymbolToSymbolTypeMap.find((**(context->GetLookahead()))->GetValue());
    if (symbol_type == kSymbolToSymbolTypeMap.end()) {
      // Default implementation but should not reach here
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
      assert(false);
    }
    auto sym = std::make_shared<ast::SymbolNode>(symbol_type->second);
    context->GetStack()->push_back(sym);
  } else if (context->IsLookaheadTypeOf<token::EndToken>()) {
    // No need to include end token in AST
    return;
  } else {
    // Default implementation but should not reach here
    auto ide = std::make_shared<ast::IdentifierNode>(
        (**(context->GetLookahead()))->GetValue());
    context->GetStack()->push_back(ide);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(false);
  }
}
} // namespace parser
