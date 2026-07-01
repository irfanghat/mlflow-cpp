#include"curl_async_client.hpp"
#include "mlflow/client.hpp"
// #include "http_transport.hpp"

namespace mlflow {

MlflowClient::MlflowClient(const std::string &base_url)
    : transport_(std::make_unique<AsyncHttpTransport>(base_url)),
      experiments_sub_(*transport_), runs_sub_(*transport_) {}

MlflowClient::~MlflowClient() = default;

void MlflowClient::process_requests()
{
  if(transport_)
  {
    transport_->process_requests();
  }
}

void MlflowClient::create_experiment(
    const std::string &name,
    std::function<void(Result<std::string>)> user_callback,
    const std::optional<std::string> &artifact_location,
    const std::optional<std::vector<ExperimentTag>> &tags) {
  return experiments_sub_.create_experiment(name, user_callback, artifact_location, tags);
}

void MlflowClient::get_experiment_by_id(const std::string &experiment_id, 
  std::function<void(Result<std::string>)> user_callback) {
  return experiments_sub_.get_experiment_by_id(experiment_id, user_callback);
}

void MlflowClient::get_experiment_by_name(const std::string &name, std::function<void(Result<std::string>)> user_callback) {
  return experiments_sub_.get_experiment_by_name(name, user_callback);
}

void MlflowClient::delete_experiment(const std::string& experiment_id, std::function<void(Result<std::string>)> user_callback)
{
  return experiments_sub_.delete_experiment(experiment_id, user_callback);
}

void MlflowClient::restore_experiment(const std::string& experiment_id, std::function<void(Result<std::string>)> user_callback)
{
  return experiments_sub_.restore_experiment(experiment_id, user_callback);
}

// Result<std::string>
// MlflowClient::update_experiment(const std::string& experiment_id, const std::string& new_name)
// {
//   return experiments_sub_.update_experiment(experiment_id, new_name);
// }

// Result<Run> MlflowClient::create_run(const std::string &experiment_id,
//                                      const TimestampMs &start_time) {
//   return runs_sub_.create_run(experiment_id, start_time);
// }

// Result<bool> MlflowClient::log_metric(const std::string &run_id,
//                                       const Metric &metric) {
//   return runs_sub_.log_metric(run_id, metric);
// }

} // namespace mlflow