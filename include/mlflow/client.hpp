#pragma once

#include "experiments.hpp"
#include "models/assessment.hpp"
#include "models/dataset.hpp"
#include "models/experiment.hpp"
#include "models/inference.hpp"
#include "models/input_tag.hpp"
#include "models/metric.hpp"
#include "models/model_output.hpp"
#include "models/param.hpp"
#include "models/result.hpp"
#include "models/run.hpp"
#include "models/run_status.hpp"
#include "models/timestamp.hpp"
#include "models/trace.hpp"
#include "models/trace_tag.hpp"
#include "runs.hpp"

#include <memory>
#include <string>

namespace mlflow {

class HttpTransport;

class MlflowClient {
public:
  explicit MlflowClient(const std::string &base_url);
  ~MlflowClient();

  Result<std::string> create_experiment(
      const std::string &name,
      const std::optional<std::string> &artifact_location = std::nullopt,
      const std::optional<std::vector<ExperimentTag>> &tags = std::nullopt);

  Result<Run> create_run(const std::string &experiment_id,
                         const TimestampMs &start_time);

  Result<bool> log_metric(const std::string &run_id, const Metric &metric);

  Result<std::string> get_experiment_by_id(const std::string &experiment_id);
  Result<std::string> get_experiment_by_name(const std::string &name);
  Result<std::string> delete_experiment(const std::string& experiment_id);

private:
  std::unique_ptr<HttpTransport> transport_;
  Experiments experiments_sub_;
  Runs runs_sub_;
};

} // namespace mlflow