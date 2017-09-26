## Installation Instructions

The only depenencies for QuickBlocks are (1) a locally-running Ethereum node (or a remote Ethereum node such as [Infura](http://infura.io)), (2) the `curl` libraries, and (3) some build tools such as `git`, `cmake`, and `python`.  Below we give specific instructions for your operating sytem followed by instructions for how to build and install QuickBlocks in the final section.

If you are the `root` user on your system, you do not need to use `sudo` below. If you are *not* the `root`, you will need to use `sudo` below.

The [ethslurp tool](../../apps/ethslurp/README.md) will not run without an API key from [EtherScan.io](http://etherscan.io/apis) (registration is required). The tool will requst a key from that site before running.

***

### for Centos

   > (sudo) yum groupinstall "Development Tools"  
   > (sudo) yum install cmake  
   > (sudo) yum install python python-devel  
   > (sudo) yum install libcurl-devel 

[Continue below](#finish) to build and install QuickBlocks

***

### for Debian

   > (sudo) apt-get install build-essential  
   > (sudo) apt-get install git   
   > (sudo) apt-get install cmake    
   > (sudo) apt-get install python python-dev  
   > (sudo) apt-get install libcurl3-dev   

[Continue below](#finish) to build and install QuickBlocks

***

### for Fedora

   > (sudo) dnf group install 'Development Tools'  
   > (sudo) dnf install gcc-c++  
   > (sudo) dnf install cmake   
   > (sudo) dnf install python python-devel   
   > (sudo) dnf install libcurl-devel   

[Continue below](#finish) to build and install QuickBlocks

***

### for FreeBSD

   > (sudo) pkg install lang/gcc49   
   > (sudo) pkg install git  
   > (sudo) pkg install cmake   
   > (sudo) pkg install python   
   > (sudo) pkg install ftp/curl   
   > cd /usr/include  
   > (sudo) ln -s ./usr/local/include/curl curl  

   >  export CPLUS\_INCLUDE\_PATH=/usr/local/include/:${CPLUS\_INCLUDE\_PATH}  
   >  export LIBRARY\_PATH=/usr/local/lib/:${LIBRARY\_PATH}  

[Continue below](#finish) to build and install QuickBlocks

***

### for Ubuntu

   > (sudo) apt-get update  
   > (sudo) apt-get upgrade  
   > (sudo) apt-get install build-essential  
   > (sudo) apt-get install libcurl3-dev   
   > (sudo) apt-get install cmake  
   > (sudo) apt-get install python python-dev  

[Continue below](#finish) to build and install QuickBlocks

***

### for MAC OSX

   > `curl/libcurl` and `python` are already installed  
   > Install `cmake` [using these instructions](https://cmake.org/download/).  
   > Install `git` [using these instructions](https://git-scm.com/download/mac).

[Continue below](#finish) to build and install QuickBlocks

***
<a name="finish" href=""></a>
### To build QuickBlocks and complete installation on all platforms

Open a terminal and change to whatever folder you normally store GitHub repos. Then:

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .  
   > cd quickBlocks  
   > mkdir build  
   > cd build  
   > cmake ../src  
   > make  
   > (sudo) make install  

