#!/usr/bin/env bash

set -euo pipefail

kind create cluster \
    --config config.yaml

kubectl wait \
    --for=condition=Ready node \
    --all \
    --timeout=180s

kubectl get nodes


# ---------------------
----------------------------
# Display cluster info.
# kubectl cluster-info --context kind-mlflow
# -------------------------------------------------