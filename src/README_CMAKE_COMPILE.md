To compile whole project using cmake do:

From the command line:

1) cd <project_path>/src
2) mkdir build
3) cd build
4) cmake ..
5) make

# If you want to clean and rebuild
1) make clean
2) make

# If the CMake files changed
1) cd <project_path>/src/build
2) rm -rf *
3) cmake ..
4) make

