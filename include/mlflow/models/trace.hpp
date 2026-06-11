#pragma once
#include "experiment.hpp"
#include "trace_tag.hpp"
#include <string>
#include <variant>
#include <vector>

namespace mlflow {

enum class TraceStatus { TRACE_STATUS_UNSPECIFIED, OK, ERROR, IN_PROGRESS };

inline std::string to_string(TraceStatus status) {
  switch (status) {
  case TraceStatus::IN_PROGRESS:
    return "IN_PROGRESS";
  case TraceStatus::OK:
    return "OK";
  case TraceStatus::ERROR:
    return "ERROR";
  case TraceStatus::TRACE_STATUS_UNSPECIFIED:
    return "TRACE_STATUS_UNSPECIFIED";
  }
  return "IN_PROGRESS";
}

inline TraceStatus run_status_from_string(const std::string &status_str) {
  if (status_str == "ERROR")
    return TraceStatus::ERROR;
  if (status_str == "TRACE_STATUS_UNSPECIFIED")
    return TraceStatus::TRACE_STATUS_UNSPECIFIED;
  return TraceStatus::IN_PROGRESS;
}

struct TraceRequestMetadata {
  std::string key;
  std::string value;
};

enum class TraceLocationType { MLFLOW_EXPERIMENT, INFERENCE_TABLE };

struct MlflowExperimentLocation {
  std::string experiment_id;
};

struct InferenceTableLocation {
  std::string catalog;
  std::string schema;
  std::string table;
};

struct TraceLocation {
  std::variant<MlflowExperimentLocation, InferenceTableLocation> location;

  TraceLocationType type() const {
    return std::visit(
        [](const auto &loc) -> TraceLocationType {
          using T = std::decay_t<decltype(loc)>;

          if constexpr (std::is_same_v<T, MlflowExperimentLocation>) {
            return TraceLocationType::MLFLOW_EXPERIMENT;
          } else {
            return TraceLocationType::INFERENCE_TABLE;
          }
        },
        location);
  }
};

struct TraceInfo {
  std::string request_id;
  std::string experiment_id;
  int64_t timestamp_ms;
  int64_t execution_time_ms;
  TraceStatus status;
  TraceRequestMetadata request_metadata;
  std::vector<TraceTag> tags;
};

struct TraceInfoV3 {
  std::string trace_id;
  std::string client_request_id;
  TraceLocation trace_location;
  std::string request;
  std::string response;
  std::string request_preview;
  std::string response_preview;
  int64_t timestamp_ms;
  int64_t execution_time_ms;
  TraceStatus status;
  TraceRequestMetadata request_metadata;
  std::vector<TraceTag> tags;
};

struct Trace {};

}; // namespace mlflow
