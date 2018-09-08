### Gold Test Cases

The gzipped archives in this folder contain what we think are the best available lists of transactions for the given monitors. The first part of each file's name 
corresponds to the folder in ./monitors subfolder. The second, obviously, is the address of the account.

You may use these lists as import data by completing the following steps:


    cd ./quickBlocks/monitors/<monitor_name>
    # clean the cache
    rm -f cache/*
    gunzip < ../../test/gold/monitors/<folder_acct.txt.gz ><acct>.txt
    cacheMan -i <acct>.txt


where `<monitor_name>` is the folder you're working with, `<folder_acct>` is the filename of the corresponding gzip file in `../../test/gold/monitors`, and `<acct>` is the Ethereum account you're importing for.

When you've done the above, you should be able to do this:

    ls -l cache
    
and you'll see two files:

    <acct>.acct.bin
    lastBlock.txt
    
From here, you should be able to do this as well:

    cacheMan --list cache/<acct>.acct.bin
    
and it will print all the blocks and transactions in the cache.
