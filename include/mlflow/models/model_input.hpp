#pragma once
#include <string>

namespace mlflow {
struct ModelInput {
  std::string name;
  std::string version;
};
} // namespace mlflow