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
#include<functional>

namespace mlflow {

// class HttpTransport;

class AsyncHttpTransport;

class MlflowClient {
public:
  explicit MlflowClient(const std::string &base_url);
  ~MlflowClient();
  
  void process_requests();

  void create_experiment(
      const std::string &name,
      std::function<void(Result<std::string>)> user_callback,
      const std::optional<std::string> &artifact_location = std::nullopt,
      const std::optional<std::vector<ExperimentTag>> &tags = std::nullopt);

  // Result<Run> create_run(const std::string &experiment_id,
  //                        const TimestampMs &start_time);

  // Result<bool> log_metric(const std::string &run_id, const Metric &metric);

  void get_experiment_by_id(const std::string &experiment_id, std::function<void(Result<std::string>)> user_callback);
  void get_experiment_by_name(const std::string &name, std::function<void(Result<std::string>)> user_callback);
  void delete_experiment(const std::string& experiment_id, std::function<void(Result<std::string>)> user_callback);
  void restore_experiment(const std::string& experiment_id, std::function<void(Result<std::string>)> user_callback);
  // Result<std::string> update_experiment(const std::string& experiment_id, const std::string& new_name);

private:
  std::unique_ptr<AsyncHttpTransport> transport_;
  Experiments experiments_sub_;
  Runs runs_sub_;
};

} // namespace mlflow