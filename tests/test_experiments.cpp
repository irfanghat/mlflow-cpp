#include "client_fixture.hpp"
#include "mlflow/models/experiment.hpp"

TEST_F(MlflowCppClientFixture, CreateExperiment) {
  std::string name = unique_name("create_experiment");

  auto res = client.create_experiment(name);

  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowCppClientFixture, CreateExperimentDuplicateShouldFail) {
  std::string name = unique_name("experiment_duplicate");

  auto first = client.create_experiment(name);
  ASSERT_TRUE(first.success);

  auto second = client.create_experiment(name);

  EXPECT_FALSE(second.success);
  EXPECT_TRUE(second.data.empty() || second.data == "");
  EXPECT_NE(second.error_message.find("HTTP"), std::string::npos);
}

TEST_F(MlflowCppClientFixture, CreateMultipleExperiments) {
  for (int i = 0; i < 5; i++) {
    auto name = unique_name("bulk_exp_" + std::to_string(i));

    auto res = client.create_experiment(name);

    ASSERT_TRUE(res.success);
    EXPECT_FALSE(res.data.empty());
  }
}

TEST_F(MlflowCppClientFixture, CreateExperimentWithArtifactLocation) {
  std::string name = unique_name("experiment_artifact_location");
  std::string artifact_location = "./custom_artifact_location";
  auto res = client.create_experiment(name, artifact_location);

  ASSERT_TRUE(res.success) << "Failed to create experiment: "
                           << res.error_message;
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowCppClientFixture, CreateExperimentWithTags) {
  std::string artifact_location = "./custom_artifact_location";
  std::string name = unique_name("experiment_with_tag");
  std::vector<mlflow::ExperimentTag> tags = {
      mlflow::ExperimentTag{"Test Tag Key", "Test Tag Value"}};
  auto res = client.create_experiment(name, artifact_location, tags);

  ASSERT_TRUE(res.success) << "Failed to create experiment: "
                           << res.error_message;
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowCppClientFixture, GetExperimentByID) {
  std::string name = unique_name("get_experiment");

  auto exp_res = client.create_experiment(name);
  auto exp_id = exp_res.data;

  ASSERT_TRUE(exp_res.success);
  EXPECT_FALSE(exp_res.data.empty());

  auto res = client.get_experiment_by_id(exp_id);
  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowCppClientFixture, GetExperimentByName) {
  std::string name = unique_name("get_experiment");
  auto exp_res = client.create_experiment(name);

  ASSERT_TRUE(exp_res.success);
  EXPECT_FALSE(exp_res.data.empty());

  auto res = client.get_experiment_by_name(name);
  ASSERT_TRUE(res.success);
  EXPECT_FALSE(res.data.empty());
}

TEST_F(MlflowCppClientFixture, DeleteExperiment)
{
  std::string name = unique_name("delete_experiment");
  auto exp_res = client.create_experiment(name);

  ASSERT_TRUE(exp_res.success);
  EXPECT_FALSE(exp_res.data.empty());

  auto res = client.delete_experiment(exp_res.data);
  ASSERT_TRUE(res.success);
  ASSERT_TRUE(res.data.empty());
}