#!/bin/bash

# This script removes old tests' results.

DIR=$1

if [ -z "$DIR" ]
then
    echo "Directory containing test results required as a parameter"
    exit 1
fi

echo "Removing old test results"

cd $DIR
if [ $? -gt 0 ]
then
    echo "Cannot access directory $DIR"
    exit 1
fi

echo $(pwd)

TO_REMOVE=`ls -1t | tail -n +4`
echo "Will remove these old test results:"
echo $TO_REMOVE

rm -rf $TO_REMOVE

echo "Removing old test results done"
