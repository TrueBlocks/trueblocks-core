acctTree argc: 2 [1:-th] 
acctTree -th 
#### Usage

`Usage:`    acctTree [-a|-s|-e|-n|-v|-h]  
`Purpose:`  Build an account tree listing first transaction, latest transaction, and node balance for each account.
        
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
| -a | --all | process all transactions from start of chain to latest block |
| -s | --start val | the first block to process |
| -e | --end val | the last block (less one) to process |
| -n | --nblocks val | the number of blocks to visit (ignored for -a) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

