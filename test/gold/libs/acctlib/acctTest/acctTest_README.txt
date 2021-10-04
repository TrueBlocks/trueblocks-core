acctTest argc: 2 [1:--readme] 
acctTest --readme 
### Usage

`Usage:`     [-a|-f|-l|-n|-v|-h]  
`Purpose:`  Build an account tree listing first transaction, latest transaction, and node balance for each account.
        
`Where:`

{{<td>}}
|          | Option                             | Description                                                      |
| -------- | ---------------------------------- | ---------------------------------------------------------------- |
| &#8208;a | &#8208;&#8208;all                  | process all transactions from start of chain to latest<br/>block |
| &#8208;f | &#8208;&#8208;first &lt;uint&gt;   | the first block to process                                       |
| &#8208;l | &#8208;&#8208;last &lt;uint&gt;    | the last block (less one) to process                             |
| &#8208;n | &#8208;&#8208;nblocks &lt;uint&gt; | the number of blocks to visit (ignored for -a)                   |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;      | export format, one of [none, json, txt, csv, api]                |
| &#8208;v | &#8208;&#8208;verbose              | set verbose level (optional level defaults to 1)                 |
| &#8208;h | &#8208;&#8208;help                 | display this help screen                                         |
{{</td>}}

