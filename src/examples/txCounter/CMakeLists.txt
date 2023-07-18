cmake_minimum_required (VERSION 3.0)

project (txCounter)

ADD_GO_INSTALLABLE_PROGRAM(txCounter *.go ${EXAMPLES_DIR})
