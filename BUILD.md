# Build Instructions

These build instructions are for MacOS and other flavors of Linux. If you want to build on Windows, it may or may not work. If you fix it, please create a pull request and we will incorporate your work.

## To Build on Most Systems:

From the command line in your development folder (i.e. ***dev_folder***):

    git clone http://github.com/Great-Hill-Corporation/quickBlocks
    cd quickBlocks
    mkdir build && cd build
    cmake ../src
    make

This should build the entire source tree. After the build, all executables appear in ***dev_folder/quickBlocks/bin***. Put that folder in your $PATH to be able to run the QuickBlocks tools.

<br>
<br><br><br><br><br><br><br><br><br><br><br><br>
<br><br><br><br><br><br><br><br><br><br><br><br>
<br><br><br><br><br><br><br><br><br><br><br><br>
---

The following is for documentation purposes only. Please ignore.

CENTOS
==========================================================================================================
Steps to cross-build quickBlocks under Linux (Centos distribution) environment:

-------------------
Requirements:
-------------------

CentOS Linux release 7.3.1611 (Core)
Derived from Red Hat Enterprise Linux 7.3 (Source)
NAME="CentOS Linux"
VERSION="7 (Core)"
ID="centos"
ID_LIKE="rhel fedora"
VERSION_ID="7"
PRETTY_NAME="CentOS Linux 7 (Core)"
ANSI_COLOR="0;31"
CPE_NAME="cpe:/o:centos:centos:7"
HOME_URL="https://www.centos.org/"
BUG_REPORT_URL="https://bugs.centos.org/"

CENTOS_MANTISBT_PROJECT="CentOS-7"
CENTOS_MANTISBT_PROJECT_VERSION="7"
REDHAT_SUPPORT_PRODUCT="centos"
REDHAT_SUPPORT_PRODUCT_VERSION="7"

CentOS Linux release 7.3.1611 (Core)
CentOS Linux release 7.3.1611 (Core)

---------------------
Steps
---------------------


1) Install basic development tools (it includes git client):

   as root user

   > yum groupinstall "Development Tools"

   or

   with sudo user

   > sudo yum groupinstall "Development Tools"

2) Install cmake:

   > yum install cmake  (as root or with sudo)

3) Install python:

   > yum install python python-devel (as root or with sudo)

4) Install CURL

   > yum install libcurl-devel (as root or with sudo)

5) Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to wokr at)
   > mkdir workspace
   > cd workspace


6) Clone github repository (master branch by default):

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .



7) Do the cmake + make steps
==========================================================================================================

UBUNTU
==========================================================================================================
Steps to cross-build quickBlocks under Linux (Ubuntu distribution) environment:

-------------------
Requirements:
-------------------

Ubuntu distribution 16.04 or above

root@ubuntu1:~# cat /etc/*rele*
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=16.04
DISTRIB_CODENAME=xenial
DISTRIB_DESCRIPTION="Ubuntu 16.04.2 LTS"
NAME="Ubuntu"
VERSION="16.04.2 LTS (Xenial Xerus)"
ID=ubuntu
ID_LIKE=debian
PRETTY_NAME="Ubuntu 16.04.2 LTS"
VERSION_ID="16.04"
HOME_URL="http://www.ubuntu.com/"
SUPPORT_URL="http://help.ubuntu.com/"
BUG_REPORT_URL="http://bugs.launchpad.net/ubuntu/"
VERSION_CODENAME=xenial
UBUNTU_CODENAME=xenial

---------------------
Steps
---------------------

1) Install basic development tools (this package includes git client):

   as root user

   > sudo apt-get install build-essential

   or

   with sudo user

   > sudo sudo apt-get install build-essential


2) Install CURL dependencies:

   apt-get install libcurl3-dev (as root or with sudo)


3) Install cmake:

   apt-get install cmake  (as root or with sudo)

4) Install python:

   > sudo apt-get install python python-dev

5) Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to wokr at)
   > mkdir workspace
   > cd workspace


6) Clone github repository (master branch by default):

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .



7) Do the cmake + make steps
==========================================================================================================
#LINUX CENTOS BUILD STEPS

Steps to cross-build quickBlocks under Linux (Centos distribution) environment:

-------------------
Requirements:
-------------------

[qb@qbcentos bin]$ cat /etc/*rele*
CentOS Linux release 7.3.1611 (Core)
Derived from Red Hat Enterprise Linux 7.3 (Source)
NAME="CentOS Linux"
VERSION="7 (Core)"
ID="centos"
ID_LIKE="rhel fedora"
VERSION_ID="7"
PRETTY_NAME="CentOS Linux 7 (Core)"
ANSI_COLOR="0;31"
CPE_NAME="cpe:/o:centos:centos:7"
HOME_URL="https://www.centos.org/"
BUG_REPORT_URL="https://bugs.centos.org/"

CENTOS_MANTISBT_PROJECT="CentOS-7"
CENTOS_MANTISBT_PROJECT_VERSION="7"
REDHAT_SUPPORT_PRODUCT="centos"
REDHAT_SUPPORT_PRODUCT_VERSION="7"

CentOS Linux release 7.3.1611 (Core)
CentOS Linux release 7.3.1611 (Core)
cpe:/o:centos:centos:7

---------------------
Steps
---------------------


1) Install basic development tools (it includes git client):

   as root user

   > yum groupinstall "Development Tools"

   or

   with sudo user

   > sudo yum groupinstall "Development Tools"

2) Install cmake:

   > yum install cmake  (as root or with sudo)

3) Install python:

   > yum install python python-devel (as root or with sudo)

4) Install CURL

   > yum install libcurl-devel (as root or with sudo)

5) Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to wokr at)
   > mkdir workspace
   > cd workspace


6) Clone github repository (master branch by default):

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .



7) Do the cmake + make steps

  >  mkdir build
  >  cd build
  >  cmake ../src
  >  make

------------------------------------------------------------- Testing some tools execution -------------------------------------------------------------------------------------------

[qb@qbcentos bin]$ ./ethprice

  Not enough arguments presented.

  Usage:    ./ethprice [-c|-f|-p|-w|-o|-t|-v|-h]  
  Purpose:  Freshen and/or print price quotes for the Ethereum network.

  Where:    
        -c  (or --clear)      Clear the database and re-download price data
        -f  (or --freshen)    Freshen database even if not needed
        -p  (or --period)     Time increment for display. Default 120 mins or :t where t is in [5|15|30|120|240|1440]
        -w  (or --when)       Time of day to start the display. :h determines the hour to start
        -o  (or --output)     Output level (default 1). :2 exposes more data per record
        -t  (or --test)       generate intermediary files but do not execute the commands
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered, in part, by the Poloniex APIs
  Powered by QuickBlocks
[qb@qbcentos bin]$ ./ethslurp

  Not enough arguments presented.

  Usage:    ./ethslurp [-a|-b|-d|-m|-n|-r|-f|-i|-e|-o|-l|-c|-t|-v|-h] addr  
  Purpose:  Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification.

  Where:    
        addr                  the address of the account or contract to slurp (required)
        -a  (or --archive)    filename of output (stdout otherwise)
        -b  (or --blocks)     export records in block range (:0[:max])
        -d  (or --dates)      export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]])
        -m  (or --max)        maximum transactions to slurp (:250000)
        -n  (or --name)       name this address
        -r  (or --rerun)      re-run the most recent slurp
        -f  (or --fmt)        pretty print, optionally add ':txt,' ':csv,' or ':html'
        -i  (or --income)     include income transactions only
        -e  (or --expense)    include expenditures only
        -o  (or --open)       open the configuration file for editing
        -l  (or --list)       list previously slurped addresses in cache
        -c  (or --clear)      clear all previously cached slurps
        -t  (or --test)       generate intermediary files but do not execute the commands
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Portions of this software are Powered by Etherscan.io APIs
  Powered by QuickBlocks
[qb@qbcentos bin]$ ./getBlock

  Not enough arguments presented.

  Usage:    ./getBlock [-f|-m|-b|-p|-t|-s|-v|-h] num  
  Purpose:  Returns block(s) from local cache (default) or directly from node.

  Where:    
        num                   which block (or blocks if more than one) to retreive (or use [start-stop) for range) (required)
        -f  (or --fromNode)   retreive the block(s) directly from the running node with no processing
        -m  (or --mini)       output only data fields that exist in miniBlocks and miniTrans
        -b  (or --binary)     retrieve only quickBlocks binary blocks if they exist, retrieve nothing otherwise
        -p  (or --parity)     mimic parity output using quickBlocks (i.e. quoted hexidecimal for numbers)
        -t  (or --terse)      retreive transaction hashes instead of full transactions
        -s  (or --silent)     do not print results to screen, used for speed testing
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
[qb@qbcentos bin]$ ./jsonTest

  Not enough arguments presented.

  Usage:    ./jsonTest [-o|-t|-v|-h]  
  Purpose:  This is what the program does.

  Where:    
        -o  (or --option1)    option one
        -t  (or --thing)      option two
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
[qb@qbcentos bin]$ ./speedTest
curl_easy_perform() failed: Couldn't connect to server
[qb@qbcentos bin]$ ./stringTest
foo and bar are not equal
xfoo and xbar are not equal
foo is less than bar
xfoo is less than xbar
foo is less than or equal to bar
xfoo is less than or equal to xbar
foo is min
bar is max
green apple is not red apple
green apple is not red apple
still, green apple is an apple
and red apple is also an apple
therefore, both are apples
Please
split
this
sentence
into
tokens
Please
split
this
sentence
into
tokens
[qb@qbcentos bin]$
#LINUX FEDORA BUILD STEPS

Steps to cross-build quickBlocks under Linux (Fedora 25 x64 distribution) environment:

-------------------
Requirements:
-------------------

Fedora release 25 (Twenty Five)
NAME=Fedora
VERSION="25 (Cloud Edition)"
ID=fedora
VERSION_ID=25
PRETTY_NAME="Fedora 25 (Cloud Edition)"
ANSI_COLOR="0;34"
CPE_NAME="cpe:/o:fedoraproject:fedora:25"
HOME_URL="https://fedoraproject.org/"
BUG_REPORT_URL="https://bugzilla.redhat.com/"
REDHAT_BUGZILLA_PRODUCT="Fedora"
REDHAT_BUGZILLA_PRODUCT_VERSION=25
REDHAT_SUPPORT_PRODUCT="Fedora"
REDHAT_SUPPORT_PRODUCT_VERSION=25
PRIVACY_POLICY_URL=https://fedoraproject.org/wiki/Legal:PrivacyPolicy
VARIANT="Cloud Edition"
VARIANT_ID=cloud
Fedora release 25 (Twenty Five)
Fedora release 25 (Twenty Five)
cpe:/o:fedoraproject:fedora:25

---------------------
Steps
---------------------


1) Install basic development tools (it includes git client):

   > (sudo) dnf group install 'Development Tools'

2) Install c++ compilers:

   > sudo dnf install gcc-c++

3) Install cmake:

   > (sudo) dnf install cmake  (as root or with sudo)

4) Install python:

   > (sudo) dnf install python python-devel (as root or with sudo)

5) Install CURL

   >  (sudo) dnf install libcurl-devel (as root or with sudo)

6) Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to wokr at)
   > mkdir workspace
   > cd workspace


7) Clone github repository (master branch by default):

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .



8) Do the cmake + make steps

  >  mkdir build
  >  cd build
  >  cmake ../src
  >  make

------------------------------------------------------------- Testing tools obtained -------------------------------------------------------------------------------------------

[qb@fedora-2gb-lon1-01 bin]$ ./getLogs

  Not enough arguments presented.

  Usage:    ./getLogs [-o|-t|-v|-h]  
  Purpose:  This is what the program does.

  Where:    
        -o  (or --option1)    option one
        -t  (or --thing)      option two
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
[qb@fedora-2gb-lon1-01 bin]$ ./getReceipt
Hello world
[qb@fedora-2gb-lon1-01 bin]$ ./getTrans

  Not enough arguments presented.

  Usage:    ./getTrans [-f|-t|-v|-h] hash bn.transID bh.transID  
  Purpose:  Retrieve an Ethereum transaction from either QuickBlocks or a running node.
            --note: 'hash' and 'blockHash' must start with '0x'.  
  Where:    
        hash                  one or more hashes of Ethereum transactions, or
        bn.transID            blockNumber.transactionID of one or more Ethereum transactions, or
        bh.transID            blockHash.transactionID of one or more Ethereum transactions
        -f  (or --fromNode)   retrieve the transaction from the running node (from QuickBlocks otherwise)
        -t  (or --trace)      return the trace of the transaction in addition to regular details
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
[qb@fedora-2gb-lon1-01 bin]$ ./ethName

  Not enough arguments presented.

  Usage:    ./ethName [-a|-c|-e|-l|-m|-s|-v|-h] term [name]  
  Purpose:  Find a name given an Ethereum address, or find an address given a name.

  Where:    
        term [name]           search terms (required)
        -a  (or --addrOnly)   export only the associated address to be used as input to further commands
        -c  (or --count)      print only the count of the number of matches
        -e  (or --edit)       edit the name database
        -l  (or --list)       list all names in the database
        -m  (or --matchCase)  matches must agree in case (the default is to ignore case)
        -s  (or --source)     search 'source' field as well name and address (the default)
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
#LINUX DEBIAN BUILD STEPS

Steps to cross-build quickBlocks under Linux (Debian 8.8 distribution) environment:

-------------------
Requirements:
-------------------

qb@qbdebian:~$ cat /etc/*lea*
PRETTY_NAME="Debian GNU/Linux 8 (jessie)"
NAME="Debian GNU/Linux"
VERSION_ID="8"
VERSION="8 (jessie)"
ID=debian
HOME_URL="http://www.debian.org/"
SUPPORT_URL="http://www.debian.org/support"
BUG_REPORT_URL="https://bugs.debian.org/"

---------------------
Steps
---------------------


1) Install basic development tools:

   >  (sudo) apt-get install build-essential  (as root or with sudo)

2) Install cmake:

   > (sudo) apt-get install cmake  (as root or with sudo)

3) Install git:

   > (sudo) apt-get install git  (as root or with sudo)

4) Install CURL

   > (sudo) apt-get install libcurl3-dev  (as root or with sudo)

4) Install python and its libs:

   > (sudo) apt-get install  python python-dev  (as root or with sudo)

5) Create the workspace folder. At command line execute:

   > cd <USER_DIRECTORY>    (where USER_DIRECTORY is the directory we want to wokr at)
   > mkdir workspace
   > cd workspace


6) Clone github repository (master branch by default):

   > git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .


7) Do the cmake + make steps

  >  mkdir build
  >  cd build
  >  cmake ../src
  >  make

------------------------------------------------------------- Testing some tools execution -------------------------------------------------------------------------------------------

qb@qbdebian:~/workspace/bin$ ./getTrans

  Not enough arguments presented.

  Usage:    ./getTrans [-f|-t|-v|-h] hash bn.transID bh.transID  
  Purpose:  Retrieve an Ethereum transaction from either QuickBlocks or a running node.
            --note: 'hash' and 'blockHash' must start with '0x'.  
  Where:    
        hash                  one or more hashes of Ethereum transactions, or
        bn.transID            blockNumber.transactionID of one or more Ethereum transactions, or
        bh.transID            blockHash.transactionID of one or more Ethereum transactions
        -f  (or --fromNode)   retrieve the transaction from the running node (from QuickBlocks otherwise)
        -t  (or --trace)      return the trace of the transaction in addition to regular details
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
qb@qbdebian:~/workspace/bin$ ./sha3

  Not enough arguments presented.

  Usage:    ./sha3 [-o|-t|-v|-h]  
  Purpose:  This is what the program does.

  Where:    
        -o  (or --option1)    option one
        -t  (or --thing)      option two
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
qb@qbdebian:~/workspace/bin$ ./grabABI

  Not enough arguments presented.

  Usage:    ./grabABI [-c|-g|-e|-n|-o|-s|-t|-v|-h] addr  
  Purpose:  Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI.

  Where:    
        addr                  the address(es) of the smart contract(s) to grab (required)
        -c  (or --canoncial)  convert all types to their canoncial represenation and remove all spaces from display
        -g  (or --generate)   generate C++ code into ':dir' for all functions and events found in the ABI
        -e  (or --encode)     generate the encodings for the functions / events in the ABI
        -n  (or --noconst)    generate encodings for non-constant functions and events only (always true when generating)
        -o  (or --open)       open the ABI file for editing, download if not already present
        -s  (or --silent)     If ABI cannot be acquired, fail silently (useful for scripting)
        -t  (or --test)       generate intermediary files but do not execute the commands
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
qb@qbdebian:~/workspace/bin$ ./isContract

  Not enough arguments presented.

  Usage:    ./isContract [-d|-n|-v|-h] addr[s]  
  Purpose:  Returns 'true' or 'false' if the given address(es) hold byte code (or displays the code).

  Where:    
        addr[s]               a list of Ethereum addresses from which to check for byte code (required)
        -d  (or --display)    display the actual code at the address(es)
        -n  (or --nodiff)     return 'true' if (exactly) two Ethereum addresses have identical code
        -v  (or --verbose)    set verbose level. Follow with a number to set level (-v0 for silent)
        -h  (or --help)       display this help screen

  Powered by QuickBlocks
FEDORA COMPILATION WARNINGS


[ 27%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/receipt.cpp.o
[ 27%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/rpc.cpp.o
[ 28%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/rpcresult.cpp.o
[ 29%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/trace.cpp.o
[ 30%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/traceaction.cpp.o
[ 30%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/traceresult.cpp.o
[ 31%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/transaction.cpp.o
/home/qb/workspace/src/libs/etherlib/transaction.cpp: In function �qblocks::SFString qblocks::nextTransactionChunk_custom(const qblocks::SFString&, bool&, const void*)�:
/home/qb/workspace/src/libs/etherlib/transaction.cpp:310:17: warning: this �if� clause does not guard... [-Wmisleading-indentation]
                 if ( fieldIn % "contractAddress" ) return fromAddress(tra->receipt.contractAddress); break;
                 ^~
/home/qb/workspace/src/libs/etherlib/transaction.cpp:310:102: note: ...this statement, but the latter is misleadingly indented as if it is guarded by the �if�
                 if ( fieldIn % "contractAddress" ) return fromAddress(tra->receipt.contractAddress); break;
                                                                                                      ^~~~~
[ 32%] Building CXX object libs/etherlib/CMakeFiles/ether.dir/webapi.cpp.o
[ 32%] Linking CXX static library /home/qb/workspace/src/libs/libether.a
[ 32%] Built target ether
