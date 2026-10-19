curl -Lo ./kind https://kind.sigs.k8s.io/dl/v0.31.0/kind-linux-amd64
chmod +x kind
sudo mv kind /usr/local/bin/

kind version
docker version
kubectl version --client