#pragma once
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
#include <memory>
#include <string>

namespace mlflow {

class HttpTransport;

class Experiments {
public:
  explicit Experiments(HttpTransport &transport) : transport_(transport) {}
  Result<std::string> create_experiment(const std::string &name);

private:
  HttpTransport &transport_;
};

class Runs {
public:
  explicit Runs(HttpTransport &transport) : transport_(transport) {}
  Result<Run> create_run(const std::string &experiment_id,
                         const TimestampMs &start_time);
  Result<bool> log_metric(const std::string &run_id, const Metric &metric);

private:
  HttpTransport &transport_;
};

class MlflowClient {
public:
  explicit MlflowClient(const std::string &base_url);
  ~MlflowClient();

  Experiments &experiments() { return experiments_sub_; }
  Runs &runs() { return runs_sub_; }

private:
  std::unique_ptr<HttpTransport> transport_;
  Experiments experiments_sub_;
  Runs runs_sub_;
};

} // namespace mlflow