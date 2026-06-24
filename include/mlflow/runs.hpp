#pragma once

#include "models/metric.hpp"
#include "models/result.hpp"
#include "models/run.hpp"
#include "models/timestamp.hpp"

#include <string>

namespace mlflow {

class HttpTransport;

class Runs {
public:
  explicit Runs(HttpTransport &transport) : transport_(transport) {}

  Result<Run> create_run(const std::string &experiment_id,
                         const TimestampMs &start_time);

  Result<bool> log_metric(const std::string &run_id, const Metric &metric);

private:
  HttpTransport &transport_;
};

} // namespace mlflow