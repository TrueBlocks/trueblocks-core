#!/bin/bash

# This script removes old tests' results.

DIR=$1

if [ -z "$DIR" ]
then
    echo "Directory containing test results required as a parameter"
fi

echo "Removing old test results"

cd $DIR
echo $(pwd)

TO_REMOVE=`ls -1r | tail -n +4`
echo "Will remove this old test results:"
echo $TO_REMOVE

rm -rf $TO_REMOVE

echo "Removing old test results done"
