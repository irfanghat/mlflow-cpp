#pragma once
#include "input_tag.hpp"
#include <string>
#include <vector>

namespace mlflow {

struct ExperimentTag {
  std::string key;
  std::string value;
};

struct Experiment {
  std::string experiment_id;
  std::string name;
  std::string artifact_location;
  std::string lifecycle_stage;
  int64_t last_update_time;
  int64_t creation_time;
  std::vector<ExperimentTag> tags;
  std::string effective_trace_archival_retention;
};

}; // namespace mlflow
