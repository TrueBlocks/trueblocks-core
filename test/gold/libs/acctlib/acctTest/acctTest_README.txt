acctTest argc: 2 [1:-th] 
acctTest -th 
### Usage

`Usage:`     [-a|-f|-l|-n|-v|-h]  
`Purpose:`  Build an account tree listing first transaction, latest transaction, and node balance for each account.
        
`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -a | --all | process all transactions from start of chain to latest block |
| -f | --first &lt;uint&gt; | the first block to process |
| -l | --last &lt;uint&gt; | the last block (less one) to process |
| -n | --nblocks &lt;uint&gt; | the number of blocks to visit (ignored for -a) |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

