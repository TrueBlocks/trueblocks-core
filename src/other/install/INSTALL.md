## Installation Instructions

The only dependencies for QuickBlocks are (1) a locally-running Ethereum node (or a remote Ethereum node such as [Infura](http://infura.io)), (2) the `curl` libraries, and (3) build tools such as `git`, `cmake`, and `python`.  Below we give specific instructions for your operating sytem. Following that are instructions for how to build and install QuickBlocks and further steps.

The [ethslurp tool](../../apps/ethslurp/README.md) will not run without an API key from [EtherScan.io](http://etherscan.io/apis) (registration is required). The tool will request a key from that site before running.

Please see [this important information](RUNNING_A_NODE.md) about running or not running the Ethereum node.

***

If you are the `root` user on your system, you do not need to use `sudo` below. If you are *not* the `root` user, you will need to use `sudo`. On all non-Windows platforms, update your system first. For example, on Ubuntu, do this `apt-get update` then `apt-get upgrade`.

### for Centos

```bash
sudo yum groupinstall "Development Tools"  
sudo yum install cmake python python-devel libcurl-devel
```

[Continue below](#finish) to build and install QuickBlocks

***

### for Fedora

```bash
sudo dnf group install 'Development Tools'
sudo dnf install cmake python python-devel libcurl-devel
```

[Continue below](#finish) to build and install QuickBlocks

***

### for FreeBSD

```bash
sudo pkg install lang/gcc49 git cmake python ftp/curl
cd /usr/include  
sudo ln -s ./usr/local/include/curl curl  
export CPLUS\_INCLUDE\_PATH=/usr/local/include/:${CPLUS\_INCLUDE\_PATH}
export LIBRARY\_PATH=/usr/local/lib/:${LIBRARY\_PATH}
```

[Continue below](#finish) to build and install QuickBlocks

***

### for Ubuntu / Debian

```bash
sudo apt install build-essential git cmake python python-dev libcurl3-dev
```

[Continue below](#finish) to build and install QuickBlocks

***

### for MAC OSX

   > `curl/libcurl` and `python` are already installed  
   > Install `cmake` [using these instructions](https://cmake.org/download/).  
   > Install `git` [using these instructions](https://git-scm.com/download/mac).

[Continue below](#finish) to build and install QuickBlocks

<a name="finish" href=""></a>
***
## To Build QuickBlocks and Finish Installation (all platforms)

Open a terminal and change to whatever folder you normally store GitHub repos. Then:

```bash
git clone https://github.com/Great-Hill-Corporation/quickBlocks.git .
cd quickBlocks
mkdir build
cd build
cmake ../src
make
sudo make install
```

The installation installs the QuickBlocks command line tools in the folder /usr/local/qblocks/bin. You may wish to add this path to your ${PATH} environment variable.

***
## After building, on to the samples

After you're done building and installing the library and tools, you may wish to try your hand at building some QuickBlocks applications or tools. We've provided a number of simple samples to help you get started. Please follow [this link](/examples).
