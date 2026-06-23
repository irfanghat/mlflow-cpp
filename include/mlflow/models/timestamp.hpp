#pragma once
#include <cstdint>

namespace mlflow {

class TimestampMs {
public:
  using value_type = int64_t;

  TimestampMs() : value_(0) {}
  explicit TimestampMs(value_type v) : value_(v) {}

  value_type value() const { return value_; }

  static TimestampMs now();

private:
  value_type value_;
};

} // namespace mlflow