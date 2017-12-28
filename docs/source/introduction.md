*******************
Introduction
*******************

..  image:: img/logo.png
   :height: 140px
   :width: 120px
   :scale: 50%
   :alt: QuickBlocks logo
   :align: right

QuickBlocks is a collection of software libraries, applications, and command-line tools designed to give you quick access to the data provided by an Ethereum node.

Overview
========

After the infamous DAO hack and subsequent fork in the summer of 2016, the WhiteHats needed an accurate list of token account holders so they could be reimbursed. The ICO had taken place a month earlier, and during that sale, after an initial fifteen day period where the token cost was at a ratio of 1:100, the ratio rose in a daily step function until the end of the sale. As token holders, we were very interested, of course, in the process. After a few days the Python code that was used to generate that off-chain list of token holders was presented to the world for its review. That code took over 20 hours to generate a list of transactions on the DAO during the last fifteen days of the sale. *This bears repeating: it took 20 hours to accumulate 15 days worth of data!*. The word "Quick" in the name of our project is in direct response to this shortcoming in the ecosystem.

Architecture
============

.. index:: utillib, etherlib

In a manner very similar to the way web3.js works, QuickBlocks sits between a locally running Ethereum node (or any node, local or remote, for that matter), and delivers the Ethereum data to your application. There are two significant improvements to the web3.js path however. First, QuickBlocks caches the data locally which means that the 20 hours mentioned above is reduced to mere minutes. Secondly, if you provide QuickBlocks with the ABI to your smart contract, it can deliver what we call articulated data. By this we mean that instead of returning data in the language of the Ethereum node (blocks and transactions and receipts and logs), we return data in the language of your smart contract (transfers and votes and proposals and expenditures). This eases the burden on your dApp developers.

..  image:: img/architecture.png
   :alt: QuickBlocks logo

Whereas the web3.js library delivers nearly identical data as is retrieved from the RPC interface, making it difficult for any but the most well versed in the data to easily use it, QuickBlocks stands between the node and your application improving the data significantly in two ways: (1) it's way faster, and (2) it's translated into the language of the smart contract. See `this FAQ item <http://quickblocks.io/docs/faq.html#can-i-trust-the-data-quickblocks-produces>`_, in response the nagging question in your head about the quality and accuracy of the data.

Terminology
-----------

 - **cache** - This refers to QuickBlocks' database cache which you may store on any hard drive (an SSD drive is much preferred). The QuickBlocks cache allows us to return data between 50 and 100 times faster than one can retrieve the same data through web3.js and the RPC alone.
 - **articulated data** - QuickBlocks is able to parse and "articulate" the data returned by the RPC, so that you get votes and proposals and onTokenPurchase events rather than the current hexadecimal mess returned by web3.js and the RPC (oh, and did we mention, it's a lot faster!)
 - **block_list** - Many of our tools take a *block_list* as a command line parameter. A block_list may be one or more valid block numbers (hex or integer), a range of valid block lists, one of a list of customizable *special* blocks such as 'byzantium' or 'doafork', or any combination of the above.
 - **trans_list** - a *trans_list*, like a block_list, comes in many forms and is used by various of our command line tools. A trans_list may be one or more transaction hashes, block numbers followed by a transaction index in the block (blockNum.transID), a block hash followed by a transaction ID (blockHash.transID), or any combination. *trans_list* is used in many tools.
 - **address_list** - *address_list* items are lists of one or more Ethereum addresses which are 42 character hexadecimal strings starting with '0x'. In the near future, we hope to add 'named accounts' which will be Ethereum addresses as picked up from the ENS smart contract; however, this feature is still in the works. Stay tuned.

Methodology
-----------

QuickBlocks is old school. We abhor JSON data, which in our estimation, is literally the worst way to deliver data imaginable (although one could add random strings of arbitrary meaningless data just to make it more terrible--why does every record carry every field name--JSON data--the format endorsed by the Department of Redundancy Department!). JSON is wonderful and useful *if you're transferring data across a wire and if the receiver of the data doesn't know the data's format and if the each record may follow a different format*, but none of these things is true for the Ethereum data. In the case where the data is remote (for example if you're using Infura), then you have to receive the data in a wire-neutral format, but there's no reason to keep it that way (thus QuickBlocks caching mechanism). Here's some example code for visiting every block on the chain and printing its hash:

::

    /*-------------------------------------------------------------------------
     * Name:    Simple
     * Purpose: To provide the easiest introduction to the QuickBlocks library.
     *          Simply request a block from Infura and print it to the screen.
     *------------------------------------------------------------------------*/
    #include "etherlib.h"
    int main(int argc, const char *argv[]) {

        // Initialize the system and tell it where to find the blockchain data.
        etherlib_init("remote");

        // Request the 3,500,000th block from Infura. Store it in 'block'
        CBlock block;
        getBlock(block, 3500000);

        // Print the block to the screen
        cout << block << "\n";

        return 0;
    }

Resources
=========

+---------+----------------------------------------------------------------------------------------------------------------+
|  Title  |  Link                                                                                                          |
+=========+================================================================================================================+
| Website | `http://quickblocks.io <http://quickblocks.io>`_                                                               |
+---------+----------------------------------------------------------------------------------------------------------------+
| GitHub  | `http://github.com/Great-Hill-Corporation/quickBlocks <http://github.com/Great-Hill-Corporation/quickBlocks>`_ |
+---------+----------------------------------------------------------------------------------------------------------------+
| Twitter | `http://twitter.com/@quickBlocks <http://twitter.com/@quickBlocks>`_                                           |
+---------+----------------------------------------------------------------------------------------------------------------+
| Medium  | `http://medium.com/@tjayrush <http://medium.com/@tjayrush>`_                                                   |
+---------+----------------------------------------------------------------------------------------------------------------+

