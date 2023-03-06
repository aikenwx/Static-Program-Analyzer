#include "sp/ast/astlib.h"
#include "procedure_subparser.h"
#include "util/instance_of.h"

namespace parser {
bool ProcedureSubparser::Parse(std::shared_ptr<Context> context) {
  auto stack = context->GetStack();
  auto i = stack->rbegin();
  if (stack->size() >= 5
    && util::instance_of<ast::SymbolNode>(*i) && (std::static_pointer_cast<ast::SymbolNode>(*i))->GetType() == ast::SymbolType::kRightBrace
    && util::instance_of<ast::StatementListNode>(*std::next(i, 1))
    && util::instance_of<ast::SymbolNode>(*std::next(i, 2)) && (std::static_pointer_cast<ast::SymbolNode>(*std::next(i, 2)))->GetType() == ast::SymbolType::kLeftBrace
    && util::instance_of<ast::NameNode>(*std::next(i, 3))
    && util::instance_of<ast::IdentifierNode>(*std::next(i, 4)) && (std::static_pointer_cast<ast::IdentifierNode>(*std::next(i, 4)))->GetValue() == "procedure") {
    // P <- procedure N { S+ }
    stack->pop_back();
    std::shared_ptr<ast::StatementListNode> sl = std::static_pointer_cast<ast::StatementListNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::NameNode> n = std::static_pointer_cast<ast::NameNode>(stack->back());
    stack->pop_back();
    stack->pop_back();
    std::shared_ptr<ast::ProcedureNode> p = std::make_shared<ast::ProcedureNode>(n->GetName(), sl);
    stack->push_back(p);
    return true;
  } else {
    return Subparser::Parse(context);
  }
}
}
