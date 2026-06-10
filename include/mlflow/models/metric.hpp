#pragma once
#include <string>

namespace mlflow {
struct Metric {
  std::string key;
  double value;
  int64_t timestamp;
  int64_t step;
};
} // namespace mlflow