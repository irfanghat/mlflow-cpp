#pragma once
#include "input_tag.hpp"
#include "run_tag.hpp"
#include <string>
#include <vector>

namespace mlflow {

/// Represents a reference to data used for training, testing,
/// or evaluation during the model development process.
struct Dataset {
  /// The name of the dataset.
  /// e.g. “my.uc.table@2” “nyc-taxi-dataset”, “fantastic-elk-3”
  /// This field is required.
  std::string name;

  /// Dataset digest, e.g. an md5 hash of the dataset
  /// that uniquely identifies it within datasets of the same name.
  /// This field is required.
  std::string digest;

  /// The type of the dataset source, e.g. `databricks-uc-table`, `DBFS`, `S3`.
  /// This field is required.
  std::string source_type;

  /// Source information for the dataset.
  /// Note that the source may not exactly reproduce the dataset if
  /// it was transformed / modified before use with MLflow.
  /// This field is required.
  std::string source;

  /// The schema of the dataset. e.g.
  /// MLflow `ColSpec` JSON for a dataframe, MLflow `TensorSpec` JSON for an
  /// ndarray, or another schema format.
  std::string schema;

  /// The profile of the dataset.
  /// Summary statistics for the dataset, such as the number of rows in a table,
  /// the mean / std / mode of each column in a table, or the number of elements
  /// in an array.
  std::string profile;
};

/// Represents a dataset and input tags.
struct DatasetInput {
  /// The dataset being used as a `Run` input.
  /// This field is required (`Dataset`).
  Dataset dataset;

  /// A list of tags for the dataset input,
  /// e.g. a "context" tag with value "training" (An array of `InputTag`).
  std::vector<InputTag> tags;
};

} // namespace mlflow