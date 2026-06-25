#include "http_transport.hpp"
#include "mlflow/client.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mlflow {

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

Result<std::string> Experiments::get_experiment_by_id(const std::string &experiment_id)
{
    std::string final_endpoint = std::string("/experiments/get") + "?" + "experiment_id=" + experiment_id;
    auto res = transport_.get(final_endpoint);

    if(res.status_code != 200)
    {
      return 
      {
          .data = "",
          .success = false,
          .error_message = "HTTP " + std::to_string(res.status_code)
      };
    }

    auto res_json = json::parse(res.body);
    return 
    {
        .data = res_json["experiment"]["experiment_id"].get<std::string>(),
        .success = true,
        .error_message = ""
    };
}

Result<std::string> Experiments::get_experiment_by_name(const std::string& name)
{
  std::string final_endpoint = std::string("/experiments/get-by-name") + "?" + "experiment_name=" + name;
  auto res = transport_.get(final_endpoint);

  if(res.status_code != 200)
    {
      return 
      {
          .data = "",
          .success = false,
          .error_message = "HTTP " + std::to_string(res.status_code)
      };
    }

    auto res_json = json::parse(res.body);
    return 
    {
        .data = res_json["experiment"]["experiment_id"].get<std::string>(),
        .success = true,
        .error_message = ""
    };

}


} // namespace mlflow