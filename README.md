# mlflow-cpp

A native C++ interface to interact directly with an MLflow tracking server via its standard REST endpoints. 
Because it communicates over HTTP, network I/O latency remains bounded by the performance characteristics of the MLflow server itself. 
The primary advantages of this library are direct C++ interoperability and the elimination of Python runtime overhead during serialization.

---

## Technical Rationale and Trade-offs

Integrating MLflow into application environments like C++ inference engines, distributed systems, or embedded loops typically requires embedding a Python interpreter or managing out-of-process scripting. This provides an alternative.

---

## Project Structure

The client separates transport mechanics from data models to allow independent testing of the API logic:


```
                  [C++ Application]
                           |
┌─────────────────────────────────────────────────────┐
│              MLflow Client Public API               │
└──────────────────────────┬──────────────────────────┘
                           │ Uses
┌─────────────────────────────────────────────────────┐
│             Transport & Serialization Layer         │
└─────────────────────────────────────────────────────┘
```

## Usage Guide

### Running a Local Validation Server
To run a local MLflow instance for testing purposes, use the provided configuration:

```bash
docker-compose up -d
```

The server will listen at `http://localhost:5000`.

### Basic API Example

```cpp
#include <mlflow/client.hpp>
#include <iostream>
#include <chrono>

int main() {
    mlflow::MlflowClient client("http://127.0.0.1:5000");

    // -------------------------------
    // Create an Experiment
    // -------------------------------
    auto exp_result = client.experiments().create_experiment("Inference-Pipeline");
    
    std::string experiment_id = exp_result.value();

    int64_t start_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    // -------------------------------
    // Start a tracking run
    // -------------------------------
    auto run_result = client.runs().create_run(experiment_id, start_time);

    std::string run_id = run_result.value().info.run_id;

    // -------------------------------
    // Structure metric payload
    // -------------------------------
    mlflow::Metric accuracy_metric{
        .key = "accuracy",
        .value = 0.942,
        .timestamp = start_time,
        .step = 1
    };

    // -------------------------------
    // Post data to the REST API
    // -------------------------------
    auto log_status = client.runs().log_metric(run_id, accuracy_metric);

    return 0;
}
```

## License

This project is licensed under the Apache 2.0 License.