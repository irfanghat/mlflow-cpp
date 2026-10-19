#include "client_fixture.hpp"

TEST_F(MlflowCppClientFixture, CreateRun) {
    bool finished_creation = false;
    mlflow::Result<std::string> creation_res;
    client.create_experiment(unique_name("run_exp"), [&](const auto& res){
        creation_res = res;
        finished_creation = true;
    });
    wait_for_completion([&](){ return finished_creation; });
    ASSERT_TRUE(creation_res.success);

    bool finished_run = false;
    mlflow::Result<mlflow::Run> run_res;
    client.create_run(creation_res.data, mlflow::TimestampMs::now(), [&](const auto& res){
        run_res = res;
        finished_run = true;
    });
    wait_for_completion([&](){ return finished_run; });

  ASSERT_TRUE(run_res.success);
  EXPECT_FALSE(run_res.data.info.run_id.empty());
}

TEST_F(MlflowCppClientFixture, LogMetric) {
    bool finished_creation = false;
    mlflow::Result<mlflow::Run> creation_res;

    client.create_experiment(unique_name("metric_exp"), [&](const auto& res){
        creation_res = res;
        finished_creation = true;
    });
    wait_for_completion([&](){ return finished_creation; });
    ASSERT_TRUE(creation_res.success);

    bool finished_run = false;
    mlflow::Result<mlflow::Run> run_res;
    client.create_run(exp.data, mlflow::TimestampMs::now(), [&](const auto& res){
        run_res = res;
        finished_run = true;
    });
    wait_for_completion([&](){ return finished_run; });

    ASSERT_TRUE(run_res.success);

    mlflow::Metric metric{.key = "accuracy",
                        .value = 0.91,
                        .timestamp = mlflow::TimestampMs::now().value(),
                        .step = 1};
    
    bool finished_logs = false;
    mlflow::Result<std::string> log_res;
    client.log_metric(run_res.data.info.run_id, metric, [&](const auto& res){
        log_res = res;
        finished_logs = true;
    });
    wait_for_completion([&](){ return finished_logs; });

  EXPECT_TRUE(log_res.success);
  EXPECT_TRUE(log_res.data);
}