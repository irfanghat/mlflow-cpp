#pragma once

#include "models/metric.hpp"
#include "models/result.hpp"
#include "models/run.hpp"
#include "models/timestamp.hpp"

#include <string>

namespace mlflow {

class AsyncHttpTransport;

class Runs {
public:
  explicit Runs(AsyncHttpTransport &transport) : transport_(transport) {}

  void create_run(const std::string &experiment_id,
                         const TimestampMs &start_time,
                        std::function<void(Result<Run>)> user_callback);

  void log_metric(const std::string &run_id, const Metric &metric, std::function<void(Result<std::string>)> user_callback);

private:
  AsyncHttpTransport &transport_;
};

} // namespace mlflow