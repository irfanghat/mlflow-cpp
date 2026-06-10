/**
 * @cite https://mlflow.org/docs/latest/api_reference/rest-api.html#run
 */

#pragma once
#include "dataset.hpp"
#include "metric.hpp"
#include "model_input.hpp"
#include "model_output.hpp"
#include "param.hpp"
#include "run_status.hpp"
#include "run_tag.hpp"
#include <optional>
#include <string>
#include <vector>

namespace mlflow {

/// Metadata of a single run.
struct RunInfo {
  /// Unique identifier for the run.
  std::string run_id;

  /// Unique identifier for the run.
  /// This field will be removed in a future MLflow version.
  std::string run_uuid;

  /// The name of the run.
  std::string run_name;

  /// The experiment ID.
  std::string experiment_id;

  /// User who initiated the run.
  /// This field is `deprecated` as of MLflow 1.0, and will be removed in a
  /// future MLflow release. Use `mlflow.user` tag instead.
  std::string user_id;

  /// Current status of the run.
  RunStatus status{RunStatus::RUNNING};

  /// Unit timestamp of when the run started in milliseconds.
  int64_t start_time{0};

  /// Unit timestamp of when the run ended in milliseconds.
  int64_t end_time{0};

  /// URI of the directory where the artifacts should be uploaded.
  /// This can be a local path (starting with "/"), or a distributed
  /// file system path, like `s3://bucket/directory` or `dbfs://my/directory`.
  /// If not set, the local `./mlruns` directory is chosen.
  std::string artifact_uri;

  /// Current life cycle stage of the experiment: OneOf(`"active"`, `"deleted"`)
  std::string lifecycle_stage;
};

/// Run data (metrics, params, and tags).
struct RunData {
  /// An array of `Metric` (Run metrics).
  std::vector<Metric> metrics;

  /// An array of `Param` (Run parameters).
  std::vector<Param> params;

  /// An array of `RunTag` (Additional metadata key-value pairs).
  std::vector<RunTag> tags;
};

/// Run inputs
struct RunInputs {
  std::vector<DatasetInput> dataset_inputs;
  std::vector<ModelInput> model_inputs;
};

struct RunOutputs {
  std::vector<ModelOutput> model_outputs;
};

struct Run {
  RunInfo info;
  std::optional<RunData> data;
  std::optional<RunInputs> inputs;
  std::optional<RunOutputs> outputs;
};

} // namespace mlflow