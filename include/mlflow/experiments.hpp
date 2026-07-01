#pragma once

#include "models/experiment.hpp"
#include "models/result.hpp"

#include <optional>
#include <string>
#include <vector>
#include<functional>

namespace mlflow {

class AsyncHttpTransport;

class Experiments {
public:
  explicit Experiments(AsyncHttpTransport &transport) : transport_(transport) {}

  /**
   * @brief Create an experiment with a name.
   */
  void create_experiment(
      const std::string &name,
      std::function<void(Result<std::string>)> user_callback,
      const std::optional<std::string> &artifact_location = std::nullopt,
      const std::optional<std::vector<ExperimentTag>> &tags = std::nullopt);

  void get_experiment_by_id(const std::string &experiment_id, std::function<void(Result<std::string>)> user_callback);
  // Result<std::string> get_experiment_by_name(const std::string &name);
  // Result<std::string> delete_experiment(const std::string& experiment_id);
  // Result<std::string> restore_experiment(const std::string& experiment_id);
  // Result<std::string> update_experiment(const std::string& experiment_id, const std::string& new_name);

private:
  AsyncHttpTransport &transport_;
};

} // namespace mlflow