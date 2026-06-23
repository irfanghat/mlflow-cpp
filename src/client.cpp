#include "mlflow/client.hpp"
#include "http_transport.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mlflow {

MlflowClient::MlflowClient(const std::string &base_url)
    : transport_(std::make_unique<HttpTransport>(base_url)),
      experiments_sub_(*transport_), runs_sub_(*transport_) {}

MlflowClient::~MlflowClient() = default;

// -----------------------------------------------------
// Experiments
// -----------------------------------------------------
Result<std::string> Experiments::create_experiment(const std::string &name) {
  json payload = {{"name", name}};
  auto res = transport_.post("/experiments/create", payload.dump());

  if (res.status_code != 200) {
    return {.data = "",
            .success = false,
            .error_message = "HTTP " + std::to_string(res.status_code)};
  }

  auto res_json = json::parse(res.body);

  return {.data = res_json["experiment_id"].get<std::string>(),
          .success = true,
          .error_message = ""};
}

// -----------------------------------------------------
// Runs
// -----------------------------------------------------
Result<Run> Runs::create_run(const std::string &experiment_id,
                             const TimestampMs &start_time) {
  json payload = {{"experiment_id", experiment_id},
                  {"start_time", start_time.value()}};

  auto res = transport_.post("/runs/create", payload.dump());

  if (res.status_code != 200) {
    return {.data = Run{},
            .success = false,
            .error_message = "HTTP " + std::to_string(res.status_code)};
  }

  auto res_json = json::parse(res.body)["run"];
  auto info_json = res_json["info"];

  RunInfo info{.run_id = info_json.value("run_id", ""),
               .run_uuid = info_json.value("run_uuid", ""),
               .run_name = info_json.value("run_name", ""),
               .experiment_id = info_json.value("experiment_id", ""),
               .user_id = info_json.value("user_id", ""),
               .status =
                   run_status_from_string(info_json.value("status", "RUNNING")),
               .start_time = info_json.value("start_time", int64_t{0}),
               .end_time = info_json.value("end_time", int64_t{0}),
               .artifact_uri = info_json.value("artifact_uri", ""),
               .lifecycle_stage = info_json.value("lifecycle_stage", "")};

  Run run{};
  run.info = std::move(info);

  return {.data = std::move(run), .success = true, .error_message = ""};
}

// -----------------------------------------------------
// Metrics
// -----------------------------------------------------
Result<bool> Runs::log_metric(const std::string &run_id, const Metric &metric) {
  json payload = {{"run_id", run_id},
                  {"key", metric.key},
                  {"value", metric.value},
                  {"timestamp", metric.timestamp},
                  {"step", metric.step}};

  auto res = transport_.post("/runs/log-metric", payload.dump());

  bool is_ok = (res.status_code == 200);

  return {
      .data = is_ok, .success = is_ok, .error_message = is_ok ? "" : res.body};
}

} // namespace mlflow