#include "client_fixture.hpp"

TEST_F(MlflowCppClientFixture, CreateRun) {
  auto exp = client.create_experiment(unique_name("run_exp"));
  ASSERT_TRUE(exp.success);

  auto run = client.create_run(exp.data, mlflow::TimestampMs::now());

  ASSERT_TRUE(run.success);
  EXPECT_FALSE(run.data.info.run_id.empty());
}

TEST_F(MlflowCppClientFixture, LogMetric) {
  auto exp = client.create_experiment(unique_name("metric_exp"));
  ASSERT_TRUE(exp.success);

  auto run = client.create_run(exp.data, mlflow::TimestampMs::now());

  ASSERT_TRUE(run.success);

  mlflow::Metric metric{.key = "accuracy",
                        .value = 0.91,
                        .timestamp = mlflow::TimestampMs::now().value(),
                        .step = 1};

  auto res = client.log_metric(run.data.info.run_id, metric);

  EXPECT_TRUE(res.success);
  EXPECT_TRUE(res.data);
}