# Ubuntu installation steps


1. Update latest version of packages running:

   > (sudo) apt-get update
   > (sudo) apt-get upgrade

2. Install basic development tools (this package includes git client):


   > (sudo) sudo apt-get install build-essential


3. Install CURL dependencies:

   > (sudo) apt-get install libcurl3-dev 


4. Install cmake:

   > (sudo) apt-get install cmake

5. Install python:

   > (sudo) apt-get install python python-dev

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
