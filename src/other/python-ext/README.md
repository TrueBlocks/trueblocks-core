### Install

QuickBlocks must be built with -fPIC 
Change the src/CMakeLists.txt
starting line 46:

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fPIC")
if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wconversion -Wall -O2 -fPIC")
endif()


To install, just run:
python setup.py install
