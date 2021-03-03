#!/usr/bin/env bash

cp  -pf  "../src/other/install/docker/CMakeLists.txt"                 "../src/"
cp  -pf  "../src/other/install/docker/libs/CMakeLists.txt"            "../src/libs/"
cp  -pf  "../src/other/install/docker/tools/CMakeLists.txt"           "../src/tools/"
cp  -pf  "../src/other/install/docker/dev_tools/CMakeLists.txt"       "../src/dev_tools/"
cp  -pf  "../src/other/install/docker/other/install/qb_config.cmake"  "../src/other/install/"
# ls -lR "../src/other/install"
