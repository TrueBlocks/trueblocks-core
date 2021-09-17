#!/bin/bash

REPO=$1
COMMIT_SHA=$2
TEST_TARGET=$3

echo "Building image..."
IMAGE_ID=`docker build -q --build-arg repo=$REPO --build-arg commit_sha=$COMMIT_SHA --build-arg test_target=$TEST_TARGET .`
echo "Done. Running Docker image and tests"
docker run $IMAGE_ID