#pragma once

#include "models/experiment.hpp"
#include "models/result.hpp"

#include <optional>
#include <string>
#include <vector>

namespace mlflow {

class HttpTransport;

class Experiments {
public:
  explicit Experiments(HttpTransport &transport) : transport_(transport) {}

  /**
   * @brief Create an experiment with a name.
   */
  Result<std::string> create_experiment(
      const std::string &name,
      const std::optional<std::string> &artifact_location = std::nullopt,
      const std::optional<std::vector<ExperimentTag>> &tags = std::nullopt);

  Result<std::string> get_experiment_by_id(const std::string &experiment_id);
  Result<std::string> get_experiment_by_name(const std::string &name);
  Result<std::string> delete_experiment(const std::string& experiment_id);
  Result<std::string> restore_experiment(const std::string& experiment_id);

private:
  HttpTransport &transport_;
};

} // namespace mlflow