#!/usr/bin/env bash

rm -f mocks.tar.gz
rm -f monitors.tar.gz
cp -pR ~/Library/Application Support/TrueBlocks/mocked/monitors .
cp -pR ~/Library/Application Support/TrueBlocks/mocked/mocks .
rm -f monitors/*expt*
tar -cvf monitors.tar monitors
tar -cvf mocks.tar mocks
gzip *.tar
rm -fR mocks
rm -fR monitors
