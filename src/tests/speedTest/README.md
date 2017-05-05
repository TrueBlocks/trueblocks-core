# blockScrape
Example Ethereum block scraper using etherlib code


Things I've Done to Speed Up Read

1. Wherever possible, store files with fixed length records.
2. 1,000 fixed length records are stored per file
3. Strings (i.e. non-fixed length fields) are stored a 1,000 block string files
4. Transactions and blocks are stored separately
5. 
