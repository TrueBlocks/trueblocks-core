cmake_minimum_required (VERSION 3.5)

project (testRunner)

ADD_GO_INSTALLABLE_PROGRAM(testRunner *.go ${BIN_DIR})
