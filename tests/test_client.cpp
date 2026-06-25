#include <gtest/gtest.h>
#include <mlflow/client.hpp>
#include <mlflow/models/result.hpp>
#include <mlflow/models/timestamp.hpp>

#include <chrono>
#include <string>

static const std::string BASE_URL = "http://localhost:5000";

class MlflowClientFixture : public ::testing::Test {
protected:
  mlflow::MlflowClient client{BASE_URL};

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

TEST(MlflowClientTest, ClientConfiguration) {
  EXPECT_NO_THROW({ mlflow::MlflowClient client("http://localhost:5000"); });
}

TEST(MlflowResultTest, DataLayout) {
  mlflow::Result<int> res{.data = 42, .success = true};

  EXPECT_TRUE(res);
  ASSERT_TRUE(res.success);
  EXPECT_EQ(res.value(), 42);
  EXPECT_EQ(res.data, 42);
  EXPECT_EQ(res.success, true);
  EXPECT_EQ(res.error_message, "");
}

TEST_F(MlflowClientFixture, CreateExperiment) {
  std::string name = unique_name("create_experiment");

  auto res = client.experiments().create_experiment(name);

  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowClientFixture, CreateExperimentDuplicateShouldFail) {
  std::string name = unique_name("experiment_duplicate");

  auto first = client.experiments().create_experiment(name);
  ASSERT_TRUE(first.success);

  auto second = client.experiments().create_experiment(name);

  EXPECT_FALSE(second.success);
  EXPECT_TRUE(second.data.empty() || second.data == "");
  EXPECT_NE(second.error_message.find("HTTP"), std::string::npos);
}

TEST_F(MlflowClientFixture, CreateMultipleExperiments) {
  for (int i = 0; i < 5; i++) {
    auto name = unique_name("bulk_exp_" + std::to_string(i));

    auto res = client.experiments().create_experiment(name);

    ASSERT_TRUE(res.success);
    EXPECT_FALSE(res.data.empty());
  }
}

TEST_F(MlflowClientFixture, GetExperimentByID)
{
  std::string name = unique_name("get_experiment");

  auto exp_res = client.experiments().create_experiment(name);
  auto exp_id = exp_res.data;

  ASSERT_TRUE(exp_res.success);
  EXPECT_FALSE(exp_res.data.empty());

  auto res = client.experiments().get_experiment_by_id(exp_id);

  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowClientFixture, GetExperimentByName)
{
  std::string name = unique_name("get_experiment");
  auto exp_res = client.experiments().create_experiment(name);

  ASSERT_TRUE(exp_res.success);
  EXPECT_FALSE(exp_res.data.empty());

  auto res = client.experiments().get_experiment_by_name(name);
  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowClientFixture, CreateRun) {
  auto exp = client.experiments().create_experiment(unique_name("run_exp"));
  ASSERT_TRUE(exp.success);

  auto run = client.runs().create_run(exp.data, mlflow::TimestampMs::now());

  ASSERT_TRUE(run.success);
  EXPECT_FALSE(run.data.info.run_id.empty());
}

TEST_F(MlflowClientFixture, LogMetric) {
  auto exp = client.experiments().create_experiment(unique_name("metric_exp"));
  ASSERT_TRUE(exp.success);

  auto run = client.runs().create_run(exp.data, mlflow::TimestampMs::now());

  ASSERT_TRUE(run.success);

  mlflow::Metric metric{.key = "accuracy",
                        .value = 0.91,
                        .timestamp = mlflow::TimestampMs::now().value(),
                        .step = 1};

  auto res = client.runs().log_metric(run.data.info.run_id, metric);

  EXPECT_TRUE(res.success);
  EXPECT_TRUE(res.data);
}