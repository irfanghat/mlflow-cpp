#pragma once
#include <string>

namespace mlflow {
struct Param {
  std::string key;
  std::string value;
};
} // namespace mlflow