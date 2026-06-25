#pragma once
#include <gtest/gtest.h>
#include <mlflow/client.hpp>
#include <mlflow/models/timestamp.hpp>
#include <chrono>
#include <string>

static const std::string BASE_URL = "http://localhost:5000";

class MlflowCppClientFixture : public ::testing::Test {
protected:
  mlflow::MlflowClient client{BASE_URL};

  /// Unique name generation for `experiments`, `runs` etc.
  std::string unique_name(const std::string &base) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    return base + "_" + std::to_string(now);
  }

  mlflow::TimestampMs fixed_time() {
    return mlflow::TimestampMs(1234567890000);
  }
};