#!/bin/bash

MODELFILE=./start-file.bin
DATADIR=./data
NUM_REPLICAS=32
COUNTER=0
while test $COUNTER -lt $NUM_REPLICAS
do
	cp $MODELFILE $DATADIR/datafile-$COUNTER.bin
	COUNTER=$(expr $COUNTER + 1)
done
