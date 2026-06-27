## API Reference

### Running a Local Validation Server
To run a local MLflow instance for testing purposes, use the provided configuration:

```bash
docker-compose up -d
```

The server will listen at `http://localhost:5000`.

## Experiments

### Create Experiment

```cpp
#include <mlflow/client.hpp>

int main() 
{
    mlflow::MlflowClient client("http://127.0.0.1:5000");

    client.create_experiment("experiment_name");
}
```

### Create Experiment with Artifact Location & Tags

```cpp
#include <mlflow/client.hpp>
#include <mlflow/models/experiment.hpp>

int main()
{
    mlflow::MlflowClient client("http://127.0.0.1:5000");
    std::vector<mlflow::ExperimentTag> tags = { mlflow::ExperimentTag{"Test Tag Key", "Test Tag Value"} };
    client.create_experiment("experiment_name", "/artifact_location", tags);
}
```

### Get Experiment by Id

```cpp
#include <mlflow/client.hpp>

int main()
{
    mlflow::MlflowClient client("http://127.0.0.1:5000");
    auto experiment = client.get_experiment_by_id("experiment_id");
}
```

### Get Experiment by Name

```cpp
#include <mlflow/client.hpp>

int main()
{
    mlflow::MlflowClient client("http://127.0.0.1:5000");
    auto experiment = client.get_experiment_by_name("experiment_name");
}
```