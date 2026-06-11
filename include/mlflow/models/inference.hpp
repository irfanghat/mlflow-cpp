#pragma once
#include <string>

namespace mlflow {
struct InferenceTableLocation {
  /// Full inference table name in the form of `catalog.schema.table_name`.
  std::string full_table_name;
};
} // namespace mlflow