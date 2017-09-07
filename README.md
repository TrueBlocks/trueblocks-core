## QuickBlocks

QuickBlocks is a collection of software [libraries](src/libs), [applications](src/apps), [tools](src/tools), and [examples](src/tests) that allow you to retrieve Ethereum blockchain data (a) more quickly, (b) with higher information content, (c) in an fully decentralized way, (d) in a fully automated way, and (e) in a highly maintenance free way. (For information on building the project, [link here](BUILD.md).)

### Installation

QuickBlocks has the following external dependencies:

1. Python
2. CURL library

QuicBlocks has been tested under the following environments:

1. Linux (Ubuntu/Centos/Fedora/Debian/FreeBSD)
2. Macintosh

Next sections describe in detail the steps required to install QuickBlocks under each environment.

IMPORTANT: Those instructions that contain the (sudo) tag must be executed as root user or as sudoer.

#### Ubuntu installation


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

#### Fedora installation


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


#### Centos installation

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


#### Debian installation

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


#### FreeBSD installation

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


### More Quickly

QuickBlocks interacts with the Ethereum blockchain via the RPC interface to accumulate `blocks`, `transactions`, `receipts`, and event `logs`. Prior to storing the data, we optimize it for later retrieval. The primary goal of QuickBlocks is to return the data as quickly as possible while maintaining a fully decentralized stance. These optimizations include:

1. attaching `receipts` and `logs` directly to `transactions`,
2. identifying in-error transactions using the node's trace functionality,
3. pre-processing incoming internal transactions,
4. storing the data optimized for quick, later retrieval.

Depending on the use case, QuickBlocks is able to achieve more than 150 times speedup relative to the node's RPC interface.

### Higher Information Content

An Ethereum node knows nothing about Solidity source code. It can only return hexadecimal bytes. In order to make the data useful, each user must re-translate the data back into the language of the smart contract. For example, [this complicated RPC data](docs/example.json) is simply [a yes vote on a proposal](docs/vote.json). While QuickBlocks is able to return raw hexadecimal data identical to the RPC, by default it returns what we call `articulated data`.

QuickBlocks is able to articulate the data by using the smart contract's ABI file. If available, an ABI file contains all the information necessary to translate the RPC data back into the language of the smart contract. This functionality is implemented in the [abilib libary](src/libs/abilib) and the [grabABI](src/apps/grabABI) app. Instead of dealing with **hashes** and **merkel roots** and **sha3**, you deal with **votes** and **transfers** and **withdrawals**.

### Fully Decentralized

Unlike other block chain scrapers such as http://etherscan.io, QuickBlocks is, by default, fully decentralized. QuickBlocks interacts with only your local Ethereum node. (Although, it can, if you wish, interact with any node such as Infura). This aspect of our system distinguishes QuickBlocks from other options. If you understand and believe in the benefits of decentralization, you will understand why this is important. If you don't, you'll undervalue this aspect of our work.

### Fully Automatic (Yet Customizable)

The source code for ***all of the functionality*** mentioned above is generated automatically (i.e. programmatically) using the [grabABI](src/apps/grabABI) and [makeClass](src/apps/makeClass) apps. This makes standing up a [monitor](src/monitors/README.md) for any smart contract (or system of smart contracts) trivial. With a one line command to [chifra](src/apps/chifra/README.md), a fully functional C++ static library is created. At the same time, because the generated code is fairly simple C++ code, it is fully customizable.

### Low Maintenance

Every smart contract, once deployed, is immutable. That means that the automatically generated C++ code is also immutable (if you want it to). You could literally automatically generate a data delivery layer for your smart contract that requires no further interaction. It can simply run forever, standing off-chain, but beside your smart contract, and deliver all your contact's data at high speed.

### Structure of the Project

1. [src](src) - source code for the QuickBlocks libraries, core applications, tools, and sample monitors.
2. bin - location of primary executables generated by QuickBlocks
3. docs - documentation
