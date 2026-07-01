#include "curl_async_client.hpp"
#include "mlflow/client.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>
#include<functional>

using json = nlohmann::json;

namespace mlflow {

void Experiments::create_experiment(
    const std::string &name,
    std::function<void(Result<std::string>)> user_callback,
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

  transport_.async_post("/experiments/create", payload.dump(), [user_callback](long status_code, const std::string& body){
    if(status_code != 200)
    {
      user_callback({
        .data = "",
        .success = false,
        .error_message = "HTTP " + std::to_string(status_code)
      });
      return;
    }

    auto res_json = json::parse(body);
    user_callback({
      .data = res_json["experiment_id"].get<std::string>(),
      .success = true,
      .error_message = ""
    });

  });
}

void Experiments::get_experiment_by_id(
  const std::string &experiment_id,
  std::function<void(Result<std::string>)> user_callback) {
  std::string final_endpoint =
      std::string("/experiments/get") + "?" + "experiment_id=" + experiment_id;
  transport_.async_get(final_endpoint, [user_callback](long status_code, const std::string& body){
    if(status_code != 200)
    {
      user_callback({
        .data = "",
        .success = false,
        .error_message = "HTTP " + std::to_string(status_code)
      });
      return;
    }

    auto res_json = json::parse(body);
    std::string exp_id = res_json["experiment"]["experiment_id"].get<std::string>();

    user_callback({
      .data = exp_id,
      .success = true,
      .error_message = ""
    });

  });
}

void Experiments::get_experiment_by_name(
  const std::string &name, 
  std::function<void(Result<std::string>)> user_callback) {
  std::string final_endpoint =
      std::string("/experiments/get-by-name") + "?" + "experiment_name=" + name;
  transport_.async_get(final_endpoint, [user_callback](long status_code, const std::string& body){

    if(status_code != 200)
    {
      user_callback({
        .data = "",
        .success = false,
        .error_message = "HTTP " + std::to_string(status_code)
      });
      return;
    }

    auto res_json = json::parse(body);
    std::string exp_id = res_json["experiment"]["experiment_id"].get<std::string>();

    user_callback({
      .data = exp_id,
      .success = true,
      .error_message = ""
    });

  });
}

void Experiments::delete_experiment(
  const std::string& experiment_id, 
  std::function<void(Result<std::string>)> user_callback)
{
  json payload = {{ "experiment_id", experiment_id }};
  transport_.async_post("/experiments/delete", payload.dump(), [user_callback](long status_code, const std::string& body){

    if(status_code != 200)
    {
      user_callback({
        .data = "",
        .success = false,
        .error_message = "HTTP " + std::to_string(status_code)
      });
      return;
    }

    auto res_json = json::parse(body);
    user_callback({
      .data = "",
      .success = true,
      .error_message = ""
    });

  });
}

void Experiments::restore_experiment(
  const std::string& experiment_id,
std::function<void(Result<std::string>)> user_callback)
{
  json payload = {{ "experiment_id", experiment_id }};
  transport_.async_post("/experiments/restore", payload.dump(), [user_callback](long status_code, const std::string& body){

    if(status_code != 200)
    {
      user_callback({
        .data = "",
        .success = false,
        .error_message = "HTTP " + std::to_string(status_code)
      });
      return;
    }

    auto res_json = json::parse(body);
    user_callback({
      .data = "",
      .success = true,
      .error_message = ""
    });

  });
}

// Result<std::string>
// Experiments::update_experiment(const std::string& experiment_id, const std::string& new_name)
// {
//   json payload = {{ "experiment_id", experiment_id }, { "new_name", new_name }};
//   auto res = transport_.post("/experiments/update", payload.dump());

//   if(res.status_code != 200)
//   {
//     return
//     {
//       .data = "",
//       .success = false,
//       .error_message = ""
//     };
//   }

//   auto res_json = json::parse(res.body);
//   return
//   {
//     .data = "",
//     .success = true,
//     .error_message = ""
//   };
// }

} // namespace mlflow