# Centos installation steps

1. Install basic development tools (this package includes git client):

   > (sudo) yum groupinstall "Development Tools"  

2. Install cmake:

   > (sudo) yum install cmake  

3. Install python:

   > (sudo) yum install python python-devel  

4. Install CURL:

   > (sudo) yum install libcurl-devel 

5. Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to work at)  
   > mkdir workspace  
   > cd workspace  


6. Clone github repository:

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .  


7. Build QuickBlocks from sources:

   >  mkdir build  
   >  cd build  
   >  cmake ../src  
   >  make  

8. Install QuickBlocks SDK in the local machine:

   > (sudo) make install  

