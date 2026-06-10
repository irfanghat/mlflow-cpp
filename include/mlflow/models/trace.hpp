#pragma once
#include "input_tag.hpp"
#include <string>
#include <vector>

namespace mlflow {

struct TraceStatus {};

struct TraceRequestMetadata {};

struct TraceTag {};

struct TraceInfo {
  std::string request_id;
  std::string experiment_id;
  int64_t timestamp_ms;
  int64_t execution_time_ms;
  TraceStatus status;
  TraceRequestMetadata request_metadata;
  std::vector<TraceTag> tags;
};

struct Trace {};

}; // namespace mlflow
