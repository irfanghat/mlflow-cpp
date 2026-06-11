#pragma once

#include <string>

namespace mlflow {

template <typename T> struct Result {
  T data{};
  bool success{false};
  std::string error_message{};

  [[nodiscard]] const T &value() const { return data; }
  [[nodiscard]] T &value() { return data; }

  [[nodiscard]] explicit operator bool() const noexcept { return success; }
};

} // namespace mlflow