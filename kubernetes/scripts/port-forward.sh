#!/usr/bin/env bash

set -euo pipefail

kubectl port-forward \
    -n mlflow \
    svc/mlflow \
    5000:5000