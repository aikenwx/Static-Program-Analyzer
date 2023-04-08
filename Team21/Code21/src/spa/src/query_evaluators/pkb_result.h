#pragma once

#include <functional>
#include <variant>
#include "query/synonym.h"
#include "PKBStorageClasses/EntityClasses/Entity.h"
#include "PKBStorageClasses/RelationshipClasses/Relationship.h"

namespace qps {

template<typename Input, typename Output>
class ResultProvider {
  using Iterator = typename Input::iterator;
  using Extractor = std::function<std::optional<Output>(Iterator)>;
 private:
  std::size_t size_;
  Iterator first_;
  Iterator last_;
  Extractor extractor_ = [](Iterator first) { return *first; };
 public:

  ResultProvider(std::size_t size, Iterator first, Iterator last)
      : size_(size), first_(first), last_(last) {}

  ResultProvider(std::size_t size, Iterator first, Iterator last, Extractor extractor)
      : size_(size), first_(first), last_(last), extractor_(std::move(extractor)) {}

  auto Empty() const -> bool {
    return first_ == last_;
  }
  auto Next() -> Output {
    return extractor_(first_++);
  }
  auto Size() const -> std::size_t {
    return size_;
  }
};

using PKBProviders = std::variant<ResultProvider<std::vector<Entity *>, Entity *>,
                                  ResultProvider<std::vector<Relationship *>, Relationship *>,
                                  ResultProvider<std::vector<Relationship *>, Entity *>>;

class PKBResult {
 private:
  std::vector<Synonym> syns_;
  PKBProviders provider_;
 public:

  template<typename Input>
  PKBResult(std::vector<Synonym> syns, std::vector<Input> *results) :
      syns_(std::move(syns)), provider_(ResultProvider<std::vector<Input>, Input>(results->size(),
                                                                                  std::begin(*results),
                                                                                  std::end(*results))) {

  }

  template<typename Input, typename Output>
  PKBResult(std::vector<Synonym> syns,
            std::vector<Input> *results,
            std::function<std::optional<Output>(Input)> extractor) :
      syns_(std::move(syns)), provider_(ResultProvider<std::vector<Input>, Output>(results->size(),
                                                                                   std::begin(*results),
                                                                                   std::end(*results),
                                                                                   extractor)) {

  }

  auto GetProvider() const -> const PKBProviders & {
    return provider_;
  }

  auto AddSynonym(Synonym syn) {
    syns_.push_back(std::move(syn));
  }
};

}  // namespace qps
