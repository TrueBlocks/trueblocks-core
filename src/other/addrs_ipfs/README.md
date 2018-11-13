### Install

Pull the latest copy of the qblocks repo:

    git pull
    cd ./build
    cmake ../src
    make
    
Next, do this:

    cd ../src/other/addr_ipfs
    make

This will build `./bin/addr_query` (although it may fail, see below).

#### Building on Linux:

If you're on Linux, the build will fail. Make these changes to the `makefile`.

     6 #-------------------------------------------------
     7 # for mac builds
     8 #libs=$(libraries)/*.a /usr/lib/libcurl.dylib       <---- comment this line
     9 # for ubuntu builds
    10 libs=$(libraries)/*.a -lcurl                        <---- uncomment this line
    11

#### Downloading the indicies:
     
Start your IPFS daemon (not sure if this is needed). Next download the transaction-per-account indicies using IPS and this command:

    ./getem

This command assumes you have IPFS installed and will take about 20 minutes. It downloads more than 50 GB of data onto your hard drive. Once that's done, run this command:

    ./bin/addr_query <address> <start_block>
    
where:

    your_address:        is the address of the account you want to scrape, and  
    start_block_num:     is the block from which to start the search (you may enter '0')

After a few seconds, you should see a series of transactions in which the given address was invovled.

    block_num.tx_id,
    block_num.tx_id,
    ...

#### Displaying the data (hacky version)

To display the data you can do this. First, find the address of a smart contract. For example, 

    ethName bancor
    
returns

    {
        "symbol": "BNT",
        "name": "Bancor Network Token",
        "addr": "0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c"
    }

Using that address we can extract all the transactions in which that address was involved and put them in a file:

    bin/addrs_query 0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c 0 | xargs >bancor.txt
    
It should report

    Found 216396 transactions for account 0xbb9bc244d798123fde783fcc1c72d3bb8c189413

And now we show the transactions and traces:

    getTrans --verbose --trace --file:bancor.txt
    
#### Note

There's a tonne of code to extract these transacitons from the node and export fully-articulated JSON or .csv. More to come...
