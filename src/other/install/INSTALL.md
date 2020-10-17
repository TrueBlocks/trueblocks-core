## Installation Instructions

The only dependencies for TrueBlocks are (1) a locally-running Ethereum node (or a remote Ethereum node such as [Infura](http://infura.io) or [QuikNode](http://quiknode.io)), (2) the `curl` libraries, and (3) build tools such as `git`, `cmake`, `python`, and `go`.  Below we give specific instructions for your operating sytem. Following that are instructions for how to build and install TrueBlocks and further steps.

The [ethslurp tool](../../apps/ethslurp/README.md) will not run without an API key from [EtherScan.io](http://etherscan.io/apis) (registration is required). The tool will request a key from that site before running.

Please see [this important information](RUNNING_A_NODE.md) about running or not running the Ethereum node.

***

If you are the `root` user on your system, you do not need to use `sudo` below. If you are *not* the `root` user, you will need to use `sudo`. On all non-Windows platforms, update your system first. For example, on Ubuntu, do this `apt-get update` then `apt-get upgrade`.

### Testing
If you wish to run the tests, you should also install `jq`. [Instructions here](https://stedolan.github.io/jq/download/).
### for Centos

```bash
yum update
yum upgrade
sudo yum groupinstall "Development Tools"  
sudo yum install cmake python python-devel libcurl-devel
```

Additionally, please [install Go](https://golang.org/doc/install).

[Continue below](#finish) to build and install TrueBlocks

***

### for Fedora

```bash
sudo dnf group install 'Development Tools'
sudo dnf install cmake python python-devel libcurl-devel
yum -y install qt-devel cmake
```

Additionally, please [install Go](https://golang.org/doc/install).

[Continue below](#finish) to build and install TrueBlocks

***

### for FreeBSD

```bash
sudo pkg install lang/gcc49 git cmake python ftp/curl
cd /usr/include  
sudo ln -s ./usr/local/include/curl curl  
export CPLUS_INCLUDE_PATH=/usr/local/include/:${CPLUS_INCLUDE_PATH}
export LIBRARY_PATH=/usr/local/lib/:${LIBRARY_PATH}
```

Additionally, please [install Go](https://golang.org/doc/install).

[Continue below](#finish) to build and install TrueBlocks

***

### for Ubuntu / Debian

```bash
sudo apt install build-essential git cmake python python-dev libcurl3-dev
```

Additionally, please [install Go](https://golang.org/doc/install).

[Continue below](#finish) to build and install TrueBlocks

***

### for MAC OSX

   > `curl/libcurl` and `python` are already installed  
   > Install `cmake` [using these instructions](https://cmake.org/download/) or `brew install cmake`  
   > Install `git` [using these instructions](https://git-scm.com/download/mac).

Additionally, please [install Go](https://golang.org/doc/install).

[Continue below](#finish) to build and install TrueBlocks

<a name="finish" href=""></a>
***
## To Build TrueBlocks and Finish Installation (all platforms)

Open a terminal and change to whatever folder you normally store GitHub repos. Then:

```bash
git clone https://github.com/Great-Hill-Corporation/trueblocks-core.git
cd trueblocks-core
mkdir build
cd build
cmake ../src
make
```

The installation installs the TrueBlocks command line tools in the folder /usr/local/qblocks/bin. You may wish to add this path to your ${PATH} environment variable.

***
## After building, on to the samples

After you're done building and installing the library and tools, you may wish to try your hand at building some TrueBlocks applications or tools. We've provided a 
number of simple samples to help you get started. Please follow [this link](/src/examples).
