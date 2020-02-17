#!/bin/bash 

RANDFILES_DIR=./random_files
FILE_SIZE=1		# file size in KBytes
NUM_FILES=5
mkdir random_files
if test $? -ne 0 ; then
	echo can\'t create $RANDFILES_DIR
	exit 1
fi
COUNTER=1
while test $COUNTER -le $NUM_FILES
do
	FNAME=$RANDFILES_DIR"/random_file_"$COUNTER".dat"
	dd if=/dev/urandom of=$FNAME bs=1024 count=$FILE_SIZE 1>/dev/null 2>&1
	COUNTER=$(expr $COUNTER + 1)
done
ls -l $RANDFILES_DIR
echo sample data for $FNAME:
tail $FNAME | hexdump
