### Install

Pull the latest copy of the `qblocks` repo ([Instructions here](https://github.com/Great-Hill-Corporation/quickBlocks/blob/develop/src/other/install/INSTALL.md)). Then from the `./quickBlocks` folder:

    git pull
    mkdir build (if not already present)
    cd ../build
    cmake ../src
    make
    
Next, run this command:

    cd ../src/other/addr_ipfs

If you're on Linux, edit the `makefile` and make these changes at lines 6 through 10:

     #-------------------------------------------------
     # for mac builds
     #libs=$(libraries)/*.a /usr/lib/libcurl.dylib       <---- comment out this line
     # for ubuntu builds
     libs=$(libraries)/*.a -lcurl                        <---- uncomment this line

Save the file and then type:    

    make

This should build a file in the `bin` folder called `./bin/addr_query`.

#### Downloading the transaction indicies:
     
Next, you must install IPFS if you haven't done so aleady [(instructions here)](https://docs.ipfs.io/introduction/install/). Assuming IPFS is properly installed and working you may now download the transaction-per-account indicies using this command from the `./src/other/addrs_ipfs` folder:

    ./download_tx_index.sh

The above command assumes IPFS is working properly. You should see the download begin shortly after running this command. The download will take around 20 minutes while nearly 50 GB of data will be copied onto your hard drive. The data will be placed in a local subfolder called `./data`. Once that's done, complete this command:

    cd data ; gunzip * ; cd -
    
and then:

    ./bin/addr_query <address> <start_block>
    
where:

    address:        is the address of the account you want to scrape, and  
    start_block:    is the block from which to start the search (you may enter '0')

After a few seconds, you should see a series of transactions in which the given address was invovled.

    block_num.tx_id,
    block_num.tx_id,
    ...

Next, given the list of transactions for your address of interest, we want to extract the actual transactional data from the blockchain.

#### Extracting and displaying the data (hacky version)

In order for the following commands to work, you must be running a local Ethereum `--tracing` node (or `--tracing on --pruning archive`). You may extract and display your contract's data by following these steps:

Using the address of the smart contract you want to explore do this (this example is for the bancor token, but you may use any address):

    ./bin/addrs_query 0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c 0 | xargs >bancor.txt
    
The previous command stored the list of all transactions on your address in the file `bancor.txt`. This command should take less than a minute and show a report something like this:

    Found 216,3xx transactions for account 0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c

And now we can show the full detail, glorious detail of the transactions and traces for that account:

    getTrans --verbose --trace --file:bancor.txt
    
#### Note

This data is not `articulated`. Using other tools from QuickBlocks (`chifra`, `blockScrape`, `acctScrape`) will allow you to export articulated JSON data which is much more useful. We're working hard on making this tech available as soon as possible.
