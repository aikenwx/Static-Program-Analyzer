#include "spa.h"

#include <PKB/PKB.h>
#include <fstream>
#include <query_evaluators/QPS.h>
#include <sp/sp.h>
#include <sstream>
#include <string>

namespace spa {
void ReadFile(const std::string &filename, std::stringstream &buf) {
  // read file into string
  std::ifstream stream(filename);
  if (!stream.is_open()) {
    throw std::runtime_error("Unable to open file");
  }

  buf << stream.rdbuf();
}

void SPA::ParseFile(const std::string &filename) {
  std::stringstream buf;
  ReadFile(filename, buf);
  std::string program = buf.str();

  ParseString(program);
};

void SPA::ParseString(std::string_view program) {
  sp::SP::Process(program, &pkb_);
}

void SPA::Evaluate(std::string query, std::list<std::string> &results) {
  qps::QPS::evaluate(query, results, *pkb_.getQueryFacade());
};

void SPA::FileToQueryList(const std::string &filename,
                          std::vector<std::string> &queries) {
  std::stringstream buf;
  ReadFile(filename, buf);

  while (!buf.eof()) {
    std::string query_defs;
    std::string query_query;
    std::string trash;

    // comment
    std::getline(buf, trash, '\n');

    // defs, and actual query
    std::getline(buf, query_defs, '\n');
    std::getline(buf, query_query, '\n');

    // expected result, and timeout
    std::getline(buf, trash, '\n');
    std::getline(buf, trash, '\n');

    std::string query = query_defs + query_query;
    if (!query.empty()) {
      queries.push_back(query_defs + query_query);
    }
  }
}
} // namespace spa
