acctTest argc: 2 [1:-th] 
acctTest -th 
#### Usage

`Usage:`    acctTest [-a|-s|-e|-n|-v|-h]  
`Purpose:`  Build an account tree listing first transaction, latest transaction, and node balance for each account.
        
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -a | --all | process all transactions from start of chain to latest block |
| -s | --start <uint> | the first block to process |
| -e | --end <uint> | the last block (less one) to process |
| -n | --nblocks <uint> | the number of blocks to visit (ignored for -a) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

