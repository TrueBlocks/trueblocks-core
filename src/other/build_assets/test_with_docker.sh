#!/bin/bash
COMMIT_SHA=$1
TEST_TARGET=$2

IMAGE_ID=`docker build -q --build-arg commit_sha=$COMMIT_SHA --build-arg test_target=$TEST_TARGET .`
docker run $IMAGE_ID