#pragma once
#include <string>

namespace mlflow {

enum class RunStatus { RUNNING, FINISHED, FAILED, KILLED };

inline std::string to_string(RunStatus status) {
  switch (status) {
  case RunStatus::RUNNING:
    return "RUNNING";
  case RunStatus::FINISHED:
    return "FINISHED";
  case RunStatus::FAILED:
    return "FAILED";
  case RunStatus::KILLED:
    return "KILLED";
  }
  return "RUNNING";
}

inline RunStatus run_status_from_string(const std::string &status_str) {
  if (status_str == "FINISHED")
    return RunStatus::FINISHED;
  if (status_str == "FAILED")
    return RunStatus::FAILED;
  if (status_str == "KILLED")
    return RunStatus::KILLED;
  return RunStatus::RUNNING;
}

} // namespace mlflow