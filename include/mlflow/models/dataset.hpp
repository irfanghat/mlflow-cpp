#pragma once
#include "run_tag.hpp"
#include <string>
#include <vector>

namespace mlflow {

struct Dataset {
  std::string name;
  std::string digest;
  std::string source_type;
  std::string source;
  std::string schema;
  std::string profile;
};

struct DatasetInput {
  Dataset dataset;
  std::vector<RunTag> tags;
};

} // namespace mlflow