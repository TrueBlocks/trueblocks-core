# Fedora installation steps


1. Install basic development tools (it includes git client):

   > (sudo) dnf group install 'Development Tools'

2. Install c++ compilers:

   > (sudo) dnf install gcc-c++

3. Install cmake:

   > (sudo) dnf install cmake 

4. Install python:

   > (sudo) dnf install python python-devel (as root or with sudo)

5. Install CURL

   > (sudo) dnf install libcurl-devel 

6. Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to work at)
   > mkdir workspace
   > cd workspace


7. Clone github repository:

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .


8. Build QuickBlocks from sources:

   >  mkdir build
   >  cd build
   >  cmake ../src
   >  make

9. Install QuickBlocks SDK in the local machine:

   > (sudo) make install

