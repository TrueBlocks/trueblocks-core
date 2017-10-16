*****
FAQ
*****

How is QuickBlocks different than web3.js?
-------------------------------------------

Two ways: (1) faster data, (2) better data.

While QuickBlocks uses the same technique to acquire the data from the Ethereum node (RPC interface), it does so with two significant improvements. First, QuickBlocks
caches the data client side. This means that subsequent retrievals are significantly faster. The second improvement is that QuickBlocks can translate the
data back into the language of your smart contract. Instead of **0xc9d27afe** which is what the RPC returns, QuickBlocks returns **vote** which is what that
hex string means. The RPC returns blockchain-centric data. It cannot do anything else. It only has blockchain-centric data. By virtue of the ABI definition file, QuickBlocks
is able to return data that looks like your smart contract. Additional benefits include QuickBlocks' ability to export its data to any format including comma-separated
values (.csv), tab delimited text (.txt), or even database commands. Or, you can avoid the export function entirely and just wire up the code to feed a database
directly. With a traditional web scale databases of every transactions on your smart contract (including those that ended in error), you can build a much richer,
much more interactive website for your end users than people are building with web3.js.

Can I trust the data QuickBlocks produces?
------------------------------------------

QuickBlocks retrieves its data using the RPC, which is identical to the method that web3.js uses, so if you trust web3.js, you should trust QuickBlocks. The results are identical.
We hear your objection: everyone uses web3.js -- it's battle tested. This is why QuickBlocks includes, with each tool, a command line option called **--raw**. When asked for
**--raw** output, QuickBlocks simply returns exactly the data returned by the RPC. Many QuickBlocks tools have an additional option called **--check**.  **Check** takes the
**--raw** data and compares it, data-field-by-data-field, with the QuickBlocks data. Finally, the portions of QuickBlocks that accumulate and produce the data are open source.
If you don't believe us that it works as advertised, step through the code and convince yourself.

What is the distinction between a tool and an application?
----------------------------------------------------------

The code for the tools is intended to serve as easy-to-grasp examples and use cases for the QuickBlocks libraries. The tools are open source and therefore, can be
reviewed to see how they work and what they do. Most tools are written with only two files: a file named the same as the tool (getBlock.cpp for getBlock tool for
example) and a file called options.cpp. Options.cpp handles all the command line parsing, so you can concentrate in each case on the other file. Most of them are very short. For example, here's the entirety of the getAccounts.cpp code:

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
