#pragma once
#include <string>

namespace mlflow {
struct RunTag {
  std::string key;
  std::string value;
};
} // namespace mlflow