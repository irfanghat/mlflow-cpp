#!/bin/bash

kubectl get all -n mlflow
kubectl get pvc -n mlflow
kubectl logs deployment/mlflow -n mlflow