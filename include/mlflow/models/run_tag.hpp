#pragma once
#include <string>

namespace mlflow {

/// Tag for a `Run`.
struct RunTag {
  /// The tag key.
  std::string key;

  /// The tag value.
  std::string value;
};
} // namespace mlflow