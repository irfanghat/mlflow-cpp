#pragma once
#include <string>

namespace mlflow {
struct ModelOutput {
  std::string name;
  std::string version;
};
} // namespace mlflow