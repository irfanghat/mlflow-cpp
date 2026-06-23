#pragma once
#include <string>

namespace mlflow {
struct InferenceTableLocation {
  /// Full inference table name in the form of `catalog.schema.table`.
  std::string full_table_name;

  /// The catalog name.
  std::string catalog;

  /// The schema name.
  std::string schema;

  /// The table name.
  std::string table;

  /// Get the full table name regardless of which fields are populated.
  std::string get_full_name() const {
    if (!full_table_name.empty())
      return full_table_name;
    if (!catalog.empty() && !schema.empty() && !table.empty()) {
      return catalog + "." + schema + "." + table;
    }
    return "";
  }
};
} // namespace mlflow