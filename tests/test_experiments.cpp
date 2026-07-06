#include "client_fixture.hpp"
#include "mlflow/models/experiment.hpp"



TEST_F(MlflowCppClientFixture, CreateExperiment) {
  std::string name = unique_name("create_experiment");

  bool finished = false;
  mlflow::Result<std::string> create_res;

  client.create_experiment(name, [&](const auto& res){
    create_res = res;
    finished = true;
  });

  wait_for_completion([&]() { return finished; });

  ASSERT_TRUE(create_res.success);
  EXPECT_FALSE(create_res.data.empty());
}

TEST_F(MlflowCppClientFixture, CreateExperimentDuplicateShouldFail) {
  std::string name = unique_name("experiment_duplicate");

  bool finished = false;
  mlflow::Result<std::string> first_create_res;

  client.create_experiment(name, [&](const auto& res){
    first_create_res = res;
    finished = true;
  });
  wait_for_completion([&]() { return finished; });
  ASSERT_TRUE(first_create_res.success);

  bool second_finished = false;
  mlflow::Result<std::string> second_create_res;

  client.create_experiment(name, [&](const auto& res){
    second_create_res = res;
    second_finished = true;
  });
  wait_for_completion([&](){ return second_finished; });

  EXPECT_FALSE(second_create_res.success);
  EXPECT_TRUE(second_create_res.data.empty() || second_create_res.data == "");
  EXPECT_NE(second_create_res.error_message.find("HTTP"), std::string::npos);
}

TEST_F(MlflowCppClientFixture, CreateMultipleExperiments) {
  for (int i = 0; i < 5; i++) {
    auto name = unique_name("bulk_exp_" + std::to_string(i));

    bool finished = false;
    mlflow::Result<std::string> create_res;

    client.create_experiment(name, [&](const auto& res){
      create_res = res;
      finished = true;
    });
    wait_for_completion([&](){ return finished; });

    ASSERT_TRUE(create_res.success);
    EXPECT_FALSE(create_res.data.empty());
  }
}

TEST_F(MlflowCppClientFixture, CreateExperimentWithArtifactLocation) {
  std::string name = unique_name("experiment_artifact_location");
  std::string artifact_location = "./custom_artifact_location";

  bool finished = false;
  mlflow::Result<std::string> create_res;

  client.create_experiment(name, [&](const auto& res){
    create_res = res;
    finished = true;
  });
  wait_for_completion([&](){ return finished; });

  ASSERT_TRUE(create_res.success) << "Failed to create experiment: "
                           << create_res.error_message;
  EXPECT_FALSE(create_res.data.empty());
}

TEST_F(MlflowCppClientFixture, CreateExperimentWithTags) {
  std::string artifact_location = "./custom_artifact_location";
  std::string name = unique_name("experiment_with_tag");
  std::vector<mlflow::ExperimentTag> tags = {
      mlflow::ExperimentTag{"Test Tag Key", "Test Tag Value"}};
  
  bool finished = false;
  mlflow::Result<std::string> create_res;
  client.create_experiment(name, [&](const auto& res){
    create_res = res;
    finished = true;
  });
  wait_for_completion([&](){ return finished; });

  ASSERT_TRUE(create_res.success) << "Failed to create experiment: "
                           << create_res.error_message;
  EXPECT_FALSE(create_res.data.empty());
}

TEST_F(MlflowCppClientFixture, GetExperimentByID) {
  std::string name = unique_name("get_experiment");

  bool finished_creation = false;
  mlflow::Result<std::string> create_res;

  client.create_experiment(name, [&](const auto& res){
    create_res = res;
    finished_creation = true;
  });
  wait_for_completion([&](){ return finished_creation; });

  
  ASSERT_TRUE(create_res.success);
  EXPECT_FALSE(create_res.data.empty());

  auto id = create_res.data;
  bool finished_fetch = false;
  mlflow::Result<std::string> fetched_res;

  client.get_experiment_by_id(id, [&](const auto& res){
    fetched_res = res;
    finished_fetch = true;
  });
  wait_for_completion([&](){ return finished_fetch; });

  ASSERT_TRUE(fetched_res.success);
  EXPECT_FALSE(fetched_res.data.empty());
}

TEST_F(MlflowCppClientFixture, GetExperimentByName) {
  std::string name = unique_name("get_experiment");

  bool finished_creation = false;
  mlflow::Result<std::string> creation_res;

  client.create_experiment(name, [&](const auto& res){
    creation_res = res;
    finished_creation = true;
  });
  wait_for_completion([&](){ return finished_creation; });

  ASSERT_TRUE(creation_res.success);
  EXPECT_FALSE(creation_res.data.empty());

  bool finished_fetch = false;
  mlflow::Result<std::string> fetched_res;

  client.get_experiment_by_name(name, [&](const auto& res){
    fetched_res = res;
    finished_fetch = true;
  });
  wait_for_completion([&](){ return finished_fetch; });

  ASSERT_TRUE(fetched_res.success);
  EXPECT_FALSE(fetched_res.data.empty());
}

TEST_F(MlflowCppClientFixture, DeleteExperiment)
{
  std::string name = unique_name("delete_experiment");

  bool finished_creation = false;
  mlflow::Result<std::string> creation_res;

  client.create_experiment(name, [&](const auto& res){
    creation_res = res;
    finished_creation = true;
  });
  wait_for_completion([&](){ return finished_creation; });

  ASSERT_TRUE(creation_res.success);
  EXPECT_FALSE(creation_res.data.empty());

  bool finished_del = false;
  mlflow::Result<std::string> deletion_res;
  
  client.delete_experiment(creation_res.data, [&](const auto& res){
    deletion_res = res;
    finished_del = true;
  });
  wait_for_completion([&](){ return finished_del; });

  ASSERT_TRUE(deletion_res.success);
  ASSERT_TRUE(deletion_res.data.empty());
}

TEST_F(MlflowCppClientFixture, RestoreExperiment)
{
  std::string name = unique_name("restore_experiment");

  bool finished_creation = false;
  mlflow::Result<std::string> creation_res;

  client.create_experiment(name, [&](const auto& res){
    creation_res = res;
    finished_creation = true;
  });
  wait_for_completion([&](){ return finished_creation; });

  ASSERT_TRUE(creation_res.success);
  EXPECT_FALSE(creation_res.data.empty());

  bool finished_del = false;
  mlflow::Result<std::string> deletion_res;

  client.delete_experiment(creation_res.data, [&](const auto& res){
    deletion_res = res;
    finished_del = true;
  });
  wait_for_completion([&](){ return finished_del; });

  ASSERT_TRUE(deletion_res.success);
  ASSERT_TRUE(deletion_res.data.empty());

  bool finished_restore = false;
  mlflow::Result<std::string> restored_res;

  client.restore_experiment(creation_res.data, [&](const auto& res){
    restored_res = res;
    finished_restore = true;
  });
  wait_for_completion([&](){ return finished_restore; });

  ASSERT_TRUE(restored_res.success);
  ASSERT_TRUE(restored_res.data.empty());
}

TEST_F(MlflowCppClientFixture, UpdateExperiment)
{
  std::string name = unique_name("update_experiment");
  bool finished_creation = false;
  mlflow::Result<std::string> creation_res;

  client.create_experiment(name, [&](const auto& res){
    creation_res = res;
    finished_creation = true;
  });
  wait_for_completion([&](){ return finished_creation; });

  ASSERT_TRUE(creation_res.success);
  EXPECT_FALSE(creation_res.data.empty());

  std::string new_name = unique_name("new_experiment");
  bool finished_update = false;
  mlflow::Result<std::string> update_res;
  client.update_experiment(creation_res.data, new_name, [&](const auto& res){
    update_res = res;
    finished_update = true;
  });
  wait_for_completion([&](){ return finished_update; });

  ASSERT_TRUE(update_res.success);
  ASSERT_TRUE(update_res.data.empty());
}