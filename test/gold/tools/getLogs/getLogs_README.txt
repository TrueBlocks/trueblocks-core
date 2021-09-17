getLogs argc: 2 [1:-th] 
getLogs -th 
### Usage

`Usage:`    chifra logs [-t|-s|-a|-v|-h] &lt;tx_id&gt; [tx_id...]  
`Purpose:`  Retrieve logs for the given transaction(s).

`Where:`  

|     | Option                | Description                                                              |
| --- | --------------------- | ------------------------------------------------------------------------ |
|     | transactions          | a space-separated list of one or more transaction identifiers (required) |
| -t  | --topic &lt;hash&gt;  | filter by one or more log topics (not implemented)                       |
| -s  | --source &lt;addr&gt; | export only if the given address emitted the event (not implemented)     |
| -a  | --articulate          | articulate the retrieved data if ABIs can be found                       |
| -x  | --fmt &lt;val&gt;     | export format, one of *[ none \| json\* \| txt \| csv \| api ]*          |
| -v  | --verbose             | set verbose level (optional level defaults to 1)                         |
| -h  | --help                | display this help screen                                                 |

`Notes:`

- The `transactions` list may be one or more space-separated identifiers which are either a transaction hash,
  a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.
- This tool checks for valid input syntax, but does not check that the transaction requested actually exists.
- If the queried node does not store historical state, the results for most older transactions are undefined.
- If you specify a 32-byte hash, it will be assumed to be a transaction hash, if the transaction is
  not found, it will be used as a topic.

