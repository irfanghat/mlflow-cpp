#include <gtest/gtest.h>
#include <mlflow/client.hpp>
#include <mlflow/models/result.hpp>

TEST(MlflowClientTest, InitializationSanity) {
  EXPECT_NO_THROW({ mlflow::MlflowClient client("http://localhost:5000"); });
}

TEST(MlflowResultTest, DataLayoutSanity) {
  mlflow::Result<int> res{.data = 42, .success = true};
  EXPECT_TRUE(res);
  EXPECT_EQ(res.value(), 42);
}