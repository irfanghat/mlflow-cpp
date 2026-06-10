#pragma once
#include <string>

namespace mlflow {

/// Tag for a `Input`.
struct InputTag {
  /// The tag key.
  std::string key;

  /// The tag value.
  std::string value;
};
} // namespace mlflow