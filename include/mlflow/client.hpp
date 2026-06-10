#pragma once
#include "models/metric.hpp"
#include "models/result.hpp"
#include "models/run.hpp"
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
  Result<Run> create_run(const std::string &experiment_id, int64_t start_time);
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