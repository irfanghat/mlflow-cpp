#include "curl_async_client.hpp"
#include "mlflow/client.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mlflow {

void Runs::log_metric(const std::string &run_id, const Metric &metric, std::function<void(Result<std::string>)> user_callback) {
  json payload = {{"run_id", run_id},
                  {"key", metric.key},
                  {"value", metric.value},
                  {"timestamp", metric.timestamp},
                  {"step", metric.step}};

  transport_.async_post("/runs/log-metric", payload.dump(), [user_callback](long status_code, const std::string& body){
    bool is_ok = (status_code == 200);
    user_callback({
        .data = is_ok,
        .success = is_ok,
        .error_message = is_ok ? "" : body
    });
  });

}

} // namespace mlflow