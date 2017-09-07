# Debian installation steps

1. Install basic development tools:

   > (sudo) apt-get install build-essential


2. Install cmake:

   > (sudo) apt-get install cmake  


3. Install git:

   > (sudo) apt-get install git 


4. Install CURL

   > (sudo) apt-get install libcurl3-dev 


5. Install python and its libs:

   > (sudo) apt-get install  python python-dev  


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

