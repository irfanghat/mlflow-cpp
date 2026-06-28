#!/usr/bin/env bash

set -euo pipefail


K8S_VERSION="1.33"


sudo apt update
sudo apt install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    gpg


############################################################
# Disable swap
############################################################
echo "Disabling swap..."

sudo swapoff -a
sudo sed -i '/ swap / s/^/#/' /etc/fstab


############################################################
# Set up Kernel modules
############################################################
cat <<EOF | sudo tee /etc/modules-load.d/k8s.conf
overlay
br_netfilter
EOF

sudo modprobe overlay
sudo modprobe br_netfilter


############################################################
# Set up sysctl settings
############################################################
cat <<EOF | sudo tee /etc/sysctl.d/k8s.conf
net.bridge.bridge-nf-call-iptables = 1
net.bridge.bridge-nf-call-ip6tables = 1
net.ipv4.ip_forward = 1
EOF

sudo sysctl --system


############################################################
# Install containerd
############################################################
echo "Installing containerd..."

sudo apt install -y containerd

sudo mkdir -p /etc/containerd

containerd config default | sudo tee /etc/containerd/config.toml >/dev/null

sudo sed -i \
's/SystemdCgroup = false/SystemdCgroup = true/' \
/etc/containerd/config.toml

sudo systemctl restart containerd
sudo systemctl enable containerd


############################################################
# Kubernetes repository
############################################################
echo "Adding Kubernetes repository..."

curl -fsSL https://pkgs.k8s.io/core:/stable:/v${K8S_VERSION}/deb/Release.key \
| sudo gpg --dearmor -o /etc/apt/keyrings/kubernetes-apt-keyring.gpg

echo "deb [signed-by=/etc/apt/keyrings/kubernetes-apt-keyring.gpg] \
https://pkgs.k8s.io/core:/stable:/v${K8S_VERSION}/deb/ /" \
| sudo tee /etc/apt/sources.list.d/kubernetes.list

sudo apt update


############################################################
# Install Kubernetes
############################################################
sudo apt install -y \
    kubelet \
    kubeadm \
    kubectl

sudo apt-mark hold kubelet kubeadm kubectl

sudo systemctl enable kubelet


############################################################
# Initialize cluster
############################################################
echo "Initializing Kubernetes..."

sudo kubeadm init --pod-network-cidr=10.244.0.0/16


############################################################
# Configure kubectl
############################################################
mkdir -p "$HOME/.kube"

sudo cp /etc/kubernetes/admin.conf "$HOME/.kube/config"

sudo chown "$(id -u):$(id -g)" "$HOME/.kube/config"


############################################################
# Install Flannel networking
############################################################
kubectl apply -f \
https://github.com/flannel-io/flannel/releases/latest/download/kube-flannel.yml


############################################################
# Allow scheduling on control plane
############################################################
kubectl taint nodes --all node-role.kubernetes.io/control-plane- || true

echo
echo "-------------------------------------"
echo " Kubernetes installation complete..."
echo "-------------------------------------"
kubectl get nodes