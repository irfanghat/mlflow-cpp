#include "client_fixture.hpp"
#include <mlflow/models/result.hpp>

TEST(MlflowClientTest, ClientConfiguration) {
  EXPECT_NO_THROW({ mlflow::MlflowClient client(BASE_URL); });
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