# FreeBSD installation steps

1. Install basic development tools (it includes git client):

   > (sudo) pkg install lang/gcc49 

2. Install git:

   > (sudo) pkg install git

3. Install cmake:

   > (sudo) pkg install cmake 

4. Install python:

   > (sudo) pkg install python 

5. Install CURL

   >  (sudo) pkg install ftp/curl 
   >  cd /usr/include
   >  (sudo) ln -s ./usr/local/include/curl curl


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
   >  export CPLUS_INCLUDE_PATH=/usr/local/include/:${CPLUS_INCLUDE_PATH}
   >  export LIBRARY_PATH=/usr/local/lib/:${LIBRARY_PATH}
   >  make

9. Install QuickBlocks SDK in the local machine:

   > (sudo) make install

