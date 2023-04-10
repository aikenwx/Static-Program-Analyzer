#include "spa.h"

#include <iostream>
#include <stdexcept>

auto main(int argc, char *argv[]) -> int {
  spa::SPA spa{};

  if (argc != 3) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::string program_name = argc > 0 ? argv[0] : "profiling_helper";
    // filename, source file, query file
    std::cout << "Usage: " << program_name << " <source file> <query file>"
              << std::endl;
    return 1;
  }

  // i guess this is kinda unavoidable, and it's probably reasonably safe anyway
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  std::string source_file = argv[1];
  std::string query_file = argv[2];
  // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

  spa.ParseFile(source_file);
  std::cout << "Parsed and loaded source file." << std::endl;

  std::vector<std::string> queries;
  spa::SPA::FileToQueryList(query_file, queries);
  std::cout << "Parsed and loaded " << queries.size() << " queries." << std::endl;

  for (const auto &query : queries) {
    std::list<std::string> results;
    spa.Evaluate(query, results);
    std::cout << "Evaluated query: " << query << std::endl;
  }
}
