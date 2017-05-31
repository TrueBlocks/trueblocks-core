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
