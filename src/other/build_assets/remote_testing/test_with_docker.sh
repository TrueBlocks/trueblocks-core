#!/bin/bash

# SOME NOTES: I spent a very long time messing around with these test after I removed the entire
# Unchained Index folder on the testing machine and then did `chifra init --all`. This was finally
# fixes (after many hours) by changing folder and file permission on the Unchained Index folders.
echo "CONFIG_FILE: " $CONFIG_FILE
#cat $CONFIG_FILE
#echo "---------------------------------------------"

REPO=$1
COMMIT_SHA=$2
BRANCH=$3

if [ -z "$CONFIG_FILE" ]
then
    echo "This script requires CONFIG_FILE env variable to be defined."
    echo "It should point out to the location of the default trueblocks.toml file"
    exit 1
fi

echo "Building image..."

# Build image and save its ID
IMAGE_ID=`docker build -q --build-arg repo=$REPO --build-arg commit_sha=$COMMIT_SHA --build-arg branch=$BRANCH .`

echo "Done. Running Docker image and tests"

# Note: we are using --rm flag, which will cause removal of the container after `docker run` exits
docker run \
    --rm \
    --network=host \
    --mount type=bind,source=/home/unchained,target=/home/unchained \
    --mount type=bind,source=$CONFIG_FILE,target=/root/.local/share/trueblocks/trueBlocks.toml \
    --mount type=bind,source=$HOME/test_results/$COMMIT_SHA,target=/root/test_results \
    $IMAGE_ID

RESULT=$?

# Remove code repository
echo "Cleaning up"
cd
rm -rf testing/$COMMIT_SHA
# If we don't want to use --rm flag with docker run, the command below will remove the container
# docker container rm `docker ps -af ancestor=$IMAGE_ID --format "{{.ID}}"`
docker image rm $IMAGE_ID

exit $RESULT
