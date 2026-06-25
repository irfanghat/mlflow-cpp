#include "mlflow/client.hpp"
#include "http_transport.hpp"

namespace mlflow {

MlflowClient::MlflowClient(const std::string &base_url)
    : transport_(std::make_unique<HttpTransport>(base_url)),
      experiments_sub_(*transport_), runs_sub_(*transport_) {}

MlflowClient::~MlflowClient() = default;

Result<std::string> MlflowClient::create_experiment(
    const std::string &name,
    const std::optional<std::string> &artifact_location,
    const std::optional<std::vector<ExperimentTag>> &tags) {
  return experiments_sub_.create_experiment(name, artifact_location, tags);
}

Result<Run> MlflowClient::create_run(const std::string &experiment_id,
                                     const TimestampMs &start_time) {
  return runs_sub_.create_run(experiment_id, start_time);
}

Result<bool> MlflowClient::log_metric(const std::string &run_id,
                                      const Metric &metric) {
  return runs_sub_.log_metric(run_id, metric);
}

} // namespace mlflow