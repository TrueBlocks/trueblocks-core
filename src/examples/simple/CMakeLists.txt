cmake_minimum_required (VERSION 3.5)

project (simple)

ADD_GO_INSTALLABLE_PROGRAM(simple *.go ${EXAMPLES_DIR})
