cmake_minimum_required (VERSION 3.5)

project (sdkTester)

ADD_GO_INSTALLABLE_PROGRAM(sdkTester *.go ${BIN_DIR})
