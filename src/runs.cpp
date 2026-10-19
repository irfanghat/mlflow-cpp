#include "curl_async_client.hpp"
#include "mlflow/client.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mlflow {

void Runs::create_run(const std::string &experiment_id,
                             const TimestampMs &start_time,
                            std::function<void(Result<Run>)> user_callback) {
  json payload = {{"experiment_id", experiment_id},
                  {"start_time", start_time.value()}};

  transport_.async_post("/runs/create", payload.dump(), [user_callback](long status_code, const std::string& body){

    if(status_code != 200)
    {
        user_callback({
            .data = Run{},
            .success = false,
            .error_message = "HTTP " + std::to_string(status_code)
        });
        return;
    }

    auto res_json = json::parse(body)["run"];
    auto info_json = res_json["info"];
    
    RunInfo info
    {
        .run_id = info_json.value("run_id", ""),
        .run_uuid = info_json.value("run_uuid", ""),
        .run_name = info_json.value("run_name", ""),
        .experiment_id = info_json.value("experiment_id", ""),
        .user_id = info_json.value("user_id", ""),
        .status =
            run_status_from_string(info_json.value("status", "RUNNING")),
        .start_time = info_json.value("start_time", int64_t{0}),
        .end_time = info_json.value("end_time", int64_t{0}),
        .artifact_uri = info_json.value("artifact_uri", ""),
        .lifecycle_stage = info_json.value("lifecycle_stage", "")
    };

    Run run{};

    run.info = std::move(info);

    user_callback({
        .data = std::move(run),
        .success = true,
        .error_message = ""
    });

  });
}

} // namespace mlflow