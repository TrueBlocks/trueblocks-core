#!/usr/bin/env bash

rm -f mocks.tar.gz
rm -f monitors.tar.gz
cp -pR ~/.quickBlocks/mocked/monitors .
cp -pR ~/.quickBlocks/mocked/mocks .
rm -f monitors/*expt*
tar -cvf monitors.tar monitors
tar -cvf mocks.tar mocks
gzip *.tar
rm -fR mocks
rm -fR monitors
