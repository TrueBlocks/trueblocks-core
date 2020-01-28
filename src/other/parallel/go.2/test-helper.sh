#!/bin/bash

for FNAME in ./random_files/* ; do
	hexdump -C $FNAME | head -1
done
