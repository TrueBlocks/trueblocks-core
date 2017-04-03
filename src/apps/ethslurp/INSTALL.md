# EthSlurp - Installation and Build Instructions

### Building the Software from Source

To build the software, use these commands:

    open a command prompt or terminal window on your system.
    git clone http://github.com/Great-Hill-Corporation/ethslurp
    cd ethslurp
    make clean
    make

### Potential Build Errors

On Ubuntu, you may have to install 'curl' on your system:

    apt-get install libcurl4-gnutls-dev

Once 'curl' is installed, you must edit the makefile so that the build finds the curl library. Please see this comment for more information on linking to 'curl' on your system: https://github.com/Great-Hill-Corporation/ethslurp/issues/87#issuecomment-256775010

On the Mac, this link may help you get started:

    http://macappstore.org/curl/

### Running the Software

To run the software, navigate to the same folder as above and enter:

    ./ethslurp --version

If the version string appears, the software is ready to go. At any time you may enter --help to see the program's help
screen. If you wish, you may copy or move the executable to any folder in your $PATH.
