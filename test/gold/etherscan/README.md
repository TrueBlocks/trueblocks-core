### Gold Test Cases

The gzipped archives in this folder contain what we think are the best available lists of transactions for the given monitors. Some of them are 'final' in the sense that they've been verified against EtherScan. The verified data is in 

	00_newbium-0x814964b1bceaf24e26296d031eadf134a2ca4105.txt.gz
	00_newbium-0xc9d7fec9889690bbff0a0df758d13e5a55dd7822.txt.gz
	00_etherTip-0x89205a3a3b2a69de6dbf7f01ed13b2108b2c43e7.txt.gz
	00_gonetwork-0x423b5f62b328d0d6d44870f4eee316befa0b2df5.txt.gz

The rest of the files in this folder were extracted from QuickBlocks but not yet verified againt EtherScan.

To extract JSON data from these files do this:

    cd ./quickBlocks/monitors/<name>
    cp -p ../test/gold/monitors/<name>-<address>.txt.gz .
    gunzip *.txt.gz
    
This will create a file called `<name>-<address>.txt` in the local folder. Rename that file to `import.txt`

    mv <name>-<address>.txt import.txt
    
Now complete the import:

    cacheMan -i cache/<address>.acct.bin
    
This will create the cache and you can now export the JSON data pulled directly from your node by entering

    acctExport --fmt json --filter <address>

This should export a fully articulated JSON file containing all the data for the given account.
