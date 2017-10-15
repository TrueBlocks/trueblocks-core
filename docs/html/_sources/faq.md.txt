*****
FAQ
*****

How is this different than web3.js?
------------------------------------

Two thoughts: (1) much faster data, (2) much more useful data.

While QuickBlocks uses the same technique to acquire the data from the Ethereum node (RPC interface), it does so with two significant improvements. First, QuickBlocks caches the data clients side. This means that on subsequent retrievals the speed of access is significantly faster. The second improvement is that QuickBlocks can translate your smart contract data back into the language of your smart contract. The RPC returns hexadecimal blockchain centric data. QuickBlocks returns data that looks like your smart contract. Other benefits include QuickBlocks' ability to export its data to any format based on a format string we called a 'display string.' Display strings allow you to export the data to any format including comma-separated values (.csv), tab delimited text (.txt), or even database commands. With a traditional web 2.0 database of all transactions on your smart contract, you can build a much richer, more interactive website for your end users.

Can I trust the data QuickBlocks produces?
------------------------------------------

Do you trust web3.js data? You probably do, so you should realize that because QuickBlocks uses the same RPC interface as web3.js, the results are identical. Your objection: yeah--but everyone uses web3.js, so I know the data is correct. (This is not actually true, but let's pretend it is.) This is why QuickBlocks includes, in each tool, the ability to generate --raw output. With --raw output, QuickBlocks simply returns exactly the same data that was presented to it from the node. Many tools even have --check options which takes --raw data and compares it, data field by data field, with the QuickBlocks data. Finally, the portions of QuickBlocks that accumulate and produce the data are open source. If you don't believe us that it works as advertised, walk your way through the code and convince yourself.

What is the distinction between a tool and an application?
----------------------------------------------------------

We intend the sample code and tools to be easy to grasp use cases for the QuickBlocks library code. The tools are open source and therefore, can be reviewed to see how they work and what functions in the libraries they call. Most tools are written with only two files: a file named the same as the tool (getBlock.cpp for getBlock tool for example) and a file called options.cpp. Options.cpp handles all the command line parsing, so you can concentrate in each case on the other file. Most of them are very short. For example, here's the entirety of the getAccounts.cpp code:

::

    #include "etherlib.h"
    //-----------------------------------------------------------------------
    int main(int argc, const char *argv[]) {

       // Tell the system where the blocks are and which version to use
        etherlib_init("fastest");

        SFAddressArray addrs;
        getAccounts(addrs);
        for (uint32_t i = 0 ; i < addrs.getCount() ; i++)
            cout << addrs[i] << "\n";
        return 0;
    }


Are there any tutorials or samples?
------------------------------------

Please see our `Quick Start Guide <http://quickblocks.io/docs/quickstart.html>`_ for information on getting started.
