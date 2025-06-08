#!/bin/bash

# Exit script on any error
set -e

# Defaults auto yes to false
AUTO_YES=false

# Parse command-line options
while getopts "y" opt; do
  case $opt in
    y)
      AUTO_YES=true
      ;;
    *)
      echo "Usage: $0 [-y]"
      exit 1
      ;;
  esac
done

# Function to check if a tool is installed
check_and_install() {
    local tool=$1
    local install_command=$2

    if command -v "$tool" >/dev/null 2>&1; then
        echo "$tool is already installed."
    else
        if [ "$AUTO_YES" = true ]; then
            response="y"
        else
            read -p "$tool is not installed. Do you want to install it? (y/n): " response
        fi

        if [[ $response == "y" || $response == "Y" ]]; then
            echo "Installing $tool..."
            eval "$install_command"
        else
            echo "Skipping $tool installation."
        fi
    fi
}

# Update package lists
echo "Updating package lists..."
sudo apt update

# Check and install tools
check_and_install "make" "sudo apt install -y make"
check_and_install "gcc" "sudo apt install -y gcc"
check_and_install "libcunit1" "sudo apt-get install -y libcunit1 libcunit1-doc libcunit1-dev"
check_and_install "valgrind" "sudo apt install -y valgrind"
check_and_install "clang-tidy" "sudo apt install -y clang-tidy"
check_and_install "clang-format-15" "sudo apt-get install -y clang-format-15"
check_and_install "clang" "sudo apt install -y clang"

echo "Setup completed."
