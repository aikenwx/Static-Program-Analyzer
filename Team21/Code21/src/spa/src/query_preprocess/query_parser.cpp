#include "query_preprocess/query_parser.h"

#include <stack>
#include <cctype>

namespace qps {

QueryParser::QueryParser(std::vector<std::string> tokens_)
    : tokens{tokens_}, currentIndex{0}, selectClause{Boolean()} {}

std::string QueryParser::peek() {
  if (currentIndex < tokens.size()) {
    return tokens[currentIndex];
  }
  return "";
}

std::string QueryParser::next() {
  std::string currentToken{peek()};
  currentIndex++;
  return currentToken;
}
bool QueryParser::isEnd() {
  return (peek() == "");
}

bool QueryParser::isTokenValidInteger(std::string str) {
  for (int i = 0; i < str.length(); i++) {
    if (!std::isdigit(str[i])) {
      return false;
    }
  }
  if (str.length() > 1 && str[0] == '0') {
    return false;
  }
  return str.length() != 0;
}

bool QueryParser::isSameToken(std::string str) {
  return (peek() == str);
}

bool QueryParser::assertNextToken(std::string str) {
  if (isSameToken(str)) {
    return true;
  }
  throw QueryException(ErrorType::Syntactic, "Syntactic error. Invalid Query Syntax. Expect (" + str + ") got (" + peek() + ")");
}

Ref QueryParser::parseRef() {
  if (isSameToken("\"")) {
    next();
    std::string id = next();
    assertNextToken("\"");
    next();
    return QuotedIdentifier(id);
  } else if (Synonym::isValidSynonym(peek())) {
    return Synonym(next());
  } else if (isSameToken("_")) {
    next();
    return Underscore();
  } else if (isTokenValidInteger(peek())) {
    return std::stoi(next());
  } else {
    throw QueryException(ErrorType::Syntactic, "Invalid representation for Ref: (" + peek() + ")");
  }
}

ExpressionSpec QueryParser::parseExpressionSpec() {
  bool isPartial{};
  std::string expression{};

  if (isSameToken("\"")) {
    next();
    expression = validateExpressionHelper(next());
    expression = makePostfix(expression);
    isPartial = false;
    assertNextToken("\"");
    next();
  } else {
    assertNextToken("_");
    next();
    if (isSameToken("\"")) {
      next();
      expression = validateExpressionHelper(next());
      expression = makePostfix(expression);
      isPartial = true;
      assertNextToken("\"");
      next();
      assertNextToken("_");
      next();
    } else {
      return Underscore();
    }
  }
  return Expression(isPartial, expression);
}

std::string QueryParser::validateExpressionHelper(std::string s) {
  s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
  int i = 0;
  int last = s.length() - 1;
  int openBracketCount = 0;
  int closeBracketCount = 0;
  while (i < s.length()) {
    int next = i + 1;
    int prev = i - 1;
    //check alphanumeric
    if (isalpha(s[i])) {
      i++;
      while (i < s.length() && isalnum(s[i])) {
        i++;
      }
    }
    //check for number
    else if (isdigit(s[i])) {
      i++;
      prev = i - 1;
      if (s[prev] == '0') {
        if (i < s.length() && isdigit(s[i])) {
          throw QueryException(ErrorType::Syntactic, "Syntactic error. Expression spec contains leading zero");
        }
      }
      while (i < s.length() && isdigit(s[i])) {
        i++;
      }
      if (i <= last && isalpha(s[i])) {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. Integer has a letter in it");
      }
    }
    //check for '('
    else if (s[i] == '(' && i < last) {
      if (i > 0) {
        if (operatorHelper(s[prev]) < 0 && s[prev] != '(') {
          throw QueryException(ErrorType::Syntactic, "Syntactic error. ( is preceded by neither an operator or (");
        }
      }
      if (!isalnum(s[next]) && s[next] != '(') {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. ( is followed by neither an alphanumeric char or (");
      }
      openBracketCount += 1;
      i++;
    }
    //check for ')'
    else if (s[i] == ')') {
      if (i == 0) {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. ) cannot be first char");
      }
      else if (!isalnum(s[prev]) && s[prev] != ')') {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. ) is preceded by neither an alphanumeric char or )");
      }
      if (i != last) {
        if (operatorHelper(s[next]) < 0 && s[next] != ')') {
          throw QueryException(ErrorType::Syntactic, "Syntactic error. ) is followed by neither an operator or )");
        }
      }
      closeBracketCount += 1;
      if (closeBracketCount > openBracketCount) {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. ) is not supposed to be allowed without a opening bracket");
      }
      i++;
    }
    //check for operators
    else if (operatorHelper(s[i]) >= 0 && i < last) {
      if (i == 0) {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. Operator cannot be first char");
      }
      if (operatorHelper(s[next]) >= 0) {
        throw QueryException(ErrorType::Syntactic, "Syntactic error. operator cannot be followed by operator");
      }
      i++;
    }
    else {
      throw QueryException(ErrorType::Syntactic,
        "Syntactic error. Expression spec contains unallowed characters inside or ends with wrong character");
    }
  }
  if (openBracketCount != closeBracketCount) {
    throw QueryException(ErrorType::Syntactic, "Syntactic error. Closing brackets are insufficient");
  }
  return s;
}

Element QueryParser::parseElement() {
  Synonym synonym = Synonym(next());
  if (isSameToken(".")) {
    next();
    AttrName attrName = getAttrNameFromString(next());
    return AttrRef(synonym, attrName);
  } else {
    return synonym;
  }
}

WithRef QueryParser::parseWithRef() {
  if (isSameToken("\"")) {
    next();
    std::string id = next();
    assertNextToken("\"");
    next();
    return WithRef(QuotedIdentifier(id));
  } else if (isTokenValidInteger(peek())) {
    return WithRef(std::stoi(next()));
  } else if (Synonym::isValidSynonym(peek())) {
    Synonym synonym = Synonym(next());
    assertNextToken(".");
    next();
    AttrName attrName = getAttrNameFromString(next());
    return WithRef(AttrRef(synonym, attrName));
  } else {
    throw QueryException(ErrorType::Syntactic, "Invalid representation for WithRef: (" + peek() + ")");
  }
}

std::vector<Element> QueryParser::parseTupleSelect() {
  std::vector<Element> tuple{};
  if (isSameToken("<")) {
    next();
    tuple.push_back(parseElement());
    while (isSameToken(",")) {
      next();
      tuple.push_back(parseElement());
    }
    assertNextToken(">");
    next();
  } else {
    tuple.push_back(parseElement());
  }
  return tuple;
}

void QueryParser::parseSuchThat() {
  Relationship relationship{getRelationshipFromString(next())};
  assertNextToken("(");
  next();
  Ref arg1{parseRef()};
  assertNextToken(",");
  next();
  Ref arg2{parseRef()};
  assertNextToken(")");
  next();

  suchThatClause.push_back(SuchThatClause(relationship, arg1, arg2, declarations));
}

void QueryParser::parsePattern() {
  Synonym synonym{Synonym(next())};
  auto declaration{Declaration::findDeclarationWithSynonym(declarations, synonym)};
  auto synDE{DesignEntity::ASSIGN};
  if (declaration.has_value()) {
    synDE = declaration.value().getDesignEntity();
  }

  assertNextToken("(");
  next();
  Ref arg1{parseRef()};
  assertNextToken(",");
  next();
  ExpressionSpec arg2{parseExpressionSpec()};
  if (synDE == DesignEntity::IF) {
    assertNextToken(",");
    next();
    assertNextToken("_");
    next();
  }
  assertNextToken(")");
  next();

  patternClause.push_back(PatternClause(synonym, arg1, arg2));
}

void QueryParser::parseWith() {
  WithRef ref1{parseWithRef()};
  assertNextToken("=");
  next();
  WithRef ref2{parseWithRef()};

  withClause.push_back(WithClause(ref1, ref2));
}

bool QueryParser::parseDeclaration() {
  try {
    getDesignEntityFromString(peek());
  } catch (QueryException &e) {
    return false;
  }
  DesignEntity design_entity{getDesignEntityFromString(next())};
  std::vector<Synonym> synonym_list;

  synonym_list.push_back(Synonym(next()));

  while (!isSameToken(";")) {
    assertNextToken(",");
    next();
    synonym_list.push_back(Synonym(next()));
  }
  next();
  for (Synonym synonym : synonym_list) {
    declarations.push_back(Declaration(design_entity, synonym));
  }
  return true;
}

void QueryParser::parseSelectClause() {
  assertNextToken("Select");
  next();
  std::string select = peek();
  if (select == "BOOLEAN") {
    Synonym test_synonym{select};
    if (Declaration::findDeclarationWithSynonym(declarations, test_synonym) == std::nullopt) {
      selectClause = Boolean();
      next();
    } else {
      selectClause = parseTupleSelect();
    }
  } else {
    selectClause = parseTupleSelect();
  }
}

bool QueryParser::parseSuchThatClause() {
  if (!isSameToken("such")) {
    return false;
  }
  next();
  assertNextToken("that");
  next();

  parseSuchThat();
  while (isSameToken("and")) {
    next();
    parseSuchThat();
  }
  return true;
}

bool QueryParser::parsePatternClause() {
  if (!isSameToken("pattern")) {
    return false;
  }
  next();

  parsePattern();
  while (isSameToken("and")) {
    next();
    parsePattern();
  }
  return true;
}

bool QueryParser::parseWithClause() {
  if (!isSameToken("with")) {
    return false;
  }
  next();

  parseWith();
  while (isSameToken("and")) {
    next();
    parseWith();
  }
  return true;
}

Query QueryParser::parse() {
  while (!isEnd()) {
    if (!parseDeclaration()) {
      break;
    }
  }
  parseSelectClause();
  while (!isEnd()) {
    if (parseSuchThatClause()) {
      continue;
    } else if (parsePatternClause()) {
      continue;
    } else if (parseWithClause()) {
      continue;
    } else {
      throw QueryException(ErrorType::Syntactic, "Invalid clause, not such-that or pattern");
    }
  }
  return Query(declarations, suchThatClause, patternClause, withClause, selectClause);
}

//helper to tell precedence of operators
int QueryParser::operatorHelper(char a) {
  switch (a) {
  case '%':
  case '*':
  case '/':return 1;
  case '+':
  case '-':return 0;
  default:return -1;
  }
}

std::string QueryParser::makePostfix(std::string s) {
  std::stack<char> stck;
  std::string postfixed;
  int i = 0;
  int last = s.length() - 1;
  while (i < s.length()) {
    //check for alphanumeric
    if (isalpha(s[i])) {
      postfixed += "\"";
      postfixed += s[i];
      i++;
      while (i < s.length() && isalnum(s[i])) {
        postfixed += s[i];
        i++;
      }
      postfixed += "\"";
    }
    //check for number
    else if (isdigit(s[i])) {
      postfixed += "\"";
      postfixed += s[i];
      i++;
      while (i < s.length() && isdigit(s[i])) {
        postfixed += s[i];
        i++;
      }
      postfixed += "\"";
    }
    //check for '('
    else if (s[i] == '(') {
      stck.push(s[i]);
      i++;
    }
    //check for ')'
    else if (s[i] == ')') {
      while (stck.top() != '(') {
        postfixed += stck.top();
        stck.pop();
      }
      stck.pop();
      i++;
    }
    //check for operators
    else if (operatorHelper(s[i]) >= 0 && i < last) {
      while (!stck.empty() && (operatorHelper(s[i]) <= operatorHelper(stck.top()))) {
        postfixed += stck.top();
        stck.pop();
      }
      stck.push(s[i]);
      i++;
    }
  }
  while (!stck.empty()) {
    postfixed += stck.top();
    stck.pop();
  }
  return postfixed;
}
}
