// #include "curl_async_client.hpp"
// #include "mlflow/client.hpp"
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// namespace mlflow {

// Result<bool> Runs::log_metric(const std::string &run_id, const Metric &metric) {
//   json payload = {{"run_id", run_id},
//                   {"key", metric.key},
//                   {"value", metric.value},
//                   {"timestamp", metric.timestamp},
//                   {"step", metric.step}};

//   auto res = transport_.post("/runs/log-metric", payload.dump());

//   bool is_ok = (res.status_code == 200);

//   return {
//       .data = is_ok, .success = is_ok, .error_message = is_ok ? "" : res.body};
// }

// } // namespace mlflow