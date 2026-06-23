#include "mlflow/models/timestamp.hpp"
#include <chrono>

namespace mlflow {

TimestampMs TimestampMs::now() {
  using namespace std::chrono;

  auto now = system_clock::now().time_since_epoch();
  auto ms = duration_cast<milliseconds>(now).count();

  return TimestampMs(static_cast<int64_t>(ms));
}

} // namespace mlflow