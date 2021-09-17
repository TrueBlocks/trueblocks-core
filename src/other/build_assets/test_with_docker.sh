#!/bin/bash

REPO=$1
COMMIT_SHA=$2
TEST_TARGET=$3

if [ -z "$CONFIG_FILE" ]
then
    echo "This script requires CONFIG_FILE env variable to be defined."
    echo "It should point out to the location of the default trueblocks.toml file"
    exit 1
fi

echo "Building image..."
IMAGE_ID=`docker build -q --build-arg repo=$REPO --build-arg commit_sha=$COMMIT_SHA --build-arg test_target=$TEST_TARGET --build-arg config_file=$CONFIG_FILE .`
echo "Done. Running Docker image and tests"
docker run $IMAGE_ID --network=host --mount type=bind,source=$HOME/unchained,target=/root/unchained