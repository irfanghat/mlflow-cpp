#include "client_fixture.hpp"

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