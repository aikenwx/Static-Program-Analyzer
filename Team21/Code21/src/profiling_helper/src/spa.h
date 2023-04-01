#include "PKB/PKB.h"
#include <list>
#include <string>

namespace spa {
class SPA {
public:
  SPA() = default;
  void ParseFile(const std::string &filename);
  void ParseString(std::string_view program);
  void Evaluate(std::string query, std::list<std::string> &results);

  static void FileToQueryList(const std::string &filename,
                              std::vector<std::string> &queries);

private:
  PKB pkb_;
};
} // namespace spa
