#include "http_transport.hpp"
#include "mlflow/client.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

using json = nlohmann::json;

namespace mlflow {

Result<std::string> Experiments::create_experiment(
    const std::string &name,
    const std::optional<std::string> &artifact_location,
    const std::optional<std::vector<ExperimentTag>> &tags) {
  json payload = {{"name", name}};

  if (artifact_location.has_value()) {
    payload["artifact_location"] = artifact_location.value();
  }

  if (tags.has_value()) {
    json tags_array = json::array();
    for (const auto &tag : tags.value()) {
      tags_array.push_back({{"key", tag.key}, {"value", tag.value}});
    }
    payload["tags"] = tags_array;
  }

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

} // namespace mlflow