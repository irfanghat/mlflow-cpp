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

See [API Reference](https://github.com/irfanghat/mlflow-cpp/blob/master/docs/API_REFERENCE.md)

## License

This project is licensed under the Apache 2.0 License.