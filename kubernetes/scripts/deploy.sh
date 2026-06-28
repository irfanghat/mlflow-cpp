#!/usr/bin/env bash

set -euo pipefail

kubectl apply -f ../manifests/namespace.yaml
kubectl apply -f ../manifests/pvc.yaml
kubectl apply -f ../manifests/deployment.yaml
kubectl apply -f ../manifests/service.yaml

kubectl rollout status deployment/mlflow -n mlflow