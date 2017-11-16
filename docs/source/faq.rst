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
-----------------------------------

Please see our `Quick Start Guide <http://quickblocks.io/docs/quickstart.html>`_ for information on getting started.

More Questions...
-----------------

**Q: Do I need any API keys to run QuickBlocks?**

    QuickBlocks itself currently does not require any API keys (it runs locally after all), but one tool, called `ethslurp` does. (The reasons for this are long and varied, but let's just say "legacy".) You may
    get the API key you need for `ethslurp` at the website `http://etherscan.io/api <http://etherscan.io/api>`_. Please follow the instructions there. Once you get a key, open a command line and type
    `ethslurp <address>` (replace <address> with any valid Ethereum address). The program will ask you for your key.

**Q: May I use QuickBlocks on 32-bits machines?**

    We know of no reason why not, but we've done exactly zero testing on 32-bit machines. If you find it works or you fix anything related to this, please post a PR.

**Q: What are the hardware requirements to run quickBlocks using a local node?**

    As of November, 2017 we are able to run both a full archive Parity node (started with `parity --tracing on --pruning archive`) on an Apple Macintosh laptop with a 1 TB internal SSD (where we
    store the entire Ethereum blockchain data) and a 1 TB Samsung external SSD (where we store QuickBlocks' caches). The blockchain data takes up about 400 GB, with occasional spikes up to around
    900 GB. The QuickBlocks cache currently takes up about 500 GB with no spiking, but in the future, when we implement a future feature we call minimal impact, it will take up magnificently (three
    orders of magnitude?) less space.

**Q: What operating systems are supported?**

    Mac OSX, Ubuntu, Fedora, Debian, CentOS, FreeBSD. Please see the `installation instructions <https://github.com/Great-Hill-Corporation/quickBlocks/blob/master/src/other/install/INSTALL.md>`_ for
    more information. We currently do not have a Windows version, but if you want to make a pull request, we're all ears.

**Q: Is the product open source or must I buy a license?**

    A little bit of both. The SDK libraries and the command-line tools are free and open source. You may use them as you wish. Other parts of QuickBlocks (the applications) must be licensed as either
    installed software (if you wish to run it locally) or via an annual subscription (where we deliver the data to your application via a web API). The first scenario imposes the burden on you to run a node but
    is more secure. The second scenario takes that burden off your hands, but is less secure (because what happens your business if we disappear?) More information will be available on
    `our website <http://quickblocks.io>`_ shortly.

**Q: How may I contact you?**

    Please visit our website for `contact information <http://quickBlocks.io>`_.
