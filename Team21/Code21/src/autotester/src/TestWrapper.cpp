#include "TestWrapper.h"

#include <fstream>
#include <sstream>

#include "PKB/PKB.h"
#include "query_evaluators/QPS.h"
#include "sp/sp.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() : pkb_() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb_ = new PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // read file into string
  std::ifstream t(filename);
  if (!t.is_open()) {
    throw std::runtime_error("Unable to open file");
  }

  std::stringstream buf;
  buf << t.rdbuf();
  std::string program = buf.str();

  sp::SP::Process(program, pkb_);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  qps::QPS::evaluate(query, results, *pkb_->getQueryFacade());
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
