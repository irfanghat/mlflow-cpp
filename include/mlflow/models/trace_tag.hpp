#pragma once
#include <string>

namespace mlflow {

/// Tag for a `Trace`.
struct TraceTag {
  /// Key identifying this trace tag.
  std::string key;

  /// Value associated with this trace tag.
  std::string value;
};
} // namespace mlflow