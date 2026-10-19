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

  void wait_for_completion(std::function<bool()> condition)
  {
    auto start = std::chrono::steady_clock::now();
    while(!condition())
    {
      client.process_requests();

      auto now = std::chrono::steady_clock::now();
      
      if(std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > 5)
      {
        FAIL() << "Test timed out waiting for create_experiment response.";
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

  mlflow::TimestampMs fixed_time() {
    return mlflow::TimestampMs(1234567890000);
  }
};