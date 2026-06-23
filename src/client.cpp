#include "mlflow/client.hpp"
#include "http_transport.hpp"

namespace mlflow {

MlflowClient::MlflowClient(const std::string &base_url)
    : transport_(std::make_unique<HttpTransport>(base_url)),
      experiments_sub_(*transport_), runs_sub_(*transport_) {}

MlflowClient::~MlflowClient() = default;
} // namespace mlflow