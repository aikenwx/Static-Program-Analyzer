#include "program_node.h"

#include "exceptions/parser_error.h"

namespace ast {
// Stores procedures as a queue FIFO
void ProgramNode::AddProcedure(
    const std::shared_ptr<ProcedureNode> &procedure) {
  procedures.push_back(procedure);
}

auto ProgramNode::GetProcedures() const
    -> std::vector<std::shared_ptr<ProcedureNode>> {
  return procedures;
}

auto ProgramNode::ToString() const -> std::string {
  std::string str = "program:\n{\n";
  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    str += (*i)->ToString();
  }
  str += "}\n";
  return str;
}

auto ProgramNode::GetTotalStatementCount() -> int {
  return procedures.back()->GetEndStatementNumber();
}

auto ProgramNode::ContainsProcedure(const std::string &procedureName) -> bool {
  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    if ((*i)->GetName() == procedureName) {
      return true;
    }
  }
  return false;
}

auto ProgramNode::GetProcedure(const std::string &procedureName)
    -> std::shared_ptr<ProcedureNode> {
  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    if ((*i)->GetName() == procedureName) {
      return *i;
    }
  }

  throw exceptions::ParserError(
      "Procedure for " + procedureName +
      " not found while trying to validate procedure");
}

void ProgramNode::AcceptVisitor(design_extractor::Extractor &extractor,
                                int depth) {
  extractor.HandleProgramNode(
      std::static_pointer_cast<ProgramNode>(shared_from_this()), depth);

  for (auto i = procedures.begin(); i < procedures.end(); i++) {
    (*i)->AcceptVisitor(extractor, depth + 1);
  }
}
} // namespace ast
