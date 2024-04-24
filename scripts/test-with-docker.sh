#!/usr/bin/env bash

#------------------------------------------------
# Name:    test-with-docker.sh
# Purpose: 
# Notes:   If you ever modify the Unchained Index files
#          one the testing machine makes sure the files
#          have the correct permissions. Otherwise, the
#          tests will fail in a very confusing manner.

#------------------------------------------------
REPO=$1
COMMIT_SHA=$2
BRANCH=$3

#------------------------------------------------
if [ -z "$CONFIG_FILE" ]
then
    echo "This script requires CONFIG_FILE env variable"
    echo "to be defined. It should point out to the location"
    echo "of the default trueBlocks.toml file on your system."
    exit 1
fi

#------------------------------------------------
echo "Building the docker image..."
if [ ! -f "Dockerfile" ]; then
    echo "Dockerfile does not exist in the current directory $(pwd)."
    exit 1
fi
IMAGE_ID=$(docker build -q \
    --build-arg repo="$REPO" \
    --build-arg commit_sha="$COMMIT_SHA" \
    --build-arg branch="$BRANCH" .)
if [ $? -ne 0 ] || [ -z "$IMAGE_ID" ]; then
    echo "Failed to build Docker image."
    exit 1
fi

#------------------------------------------------
echo "Running the docker image..."
docker run \
    --rm \
    --network=host \
    --mount type=bind,source="/home/unchained",target="/home/unchained" \
    --mount type=bind,source="$CONFIG_FILE",target="/root/.local/share/trueblocks/trueBlocks.toml" \
    --mount type=bind,source="$HOME/test_results/$COMMIT_SHA",target="/root/test_results" \
    "$IMAGE_ID"
RESULT=$?

#------------------------------------------------
echo "Cleaning up..."
rm -rf "$HOME/testing/$COMMIT_SHA" || echo "Warning: Failed to remove $HOME/testing/$COMMIT_SHA"
docker image rm "$IMAGE_ID" || echo "Warning: Failed to remove Docker image $IMAGE_ID"

exit $RESULT
