# MAC OSX installation steps


1. All versions of OS X starting with Jaguar (released ten years ago) come with curl/libcurl installed, so no need to install CURL dependencies


2. Python comes also pre-installed so no need to install it neither.


4. Install cmake using binaries distribution available here:

   https://cmake.org/download/

5. Install git from this URL:

   https://git-scm.com/download/mac

6. Open a terminal and create the workspace folder. At command line execute:

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
