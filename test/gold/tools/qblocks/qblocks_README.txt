qblocks argc: 2 [1:-th] 
qblocks -th 
#### Usage

`Usage:`    qblocks [-h] &lt;cmd&gt; &lt;sub_cmds&gt;  
`Purpose:`  Overarching command for all QBlocks tools.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | cmd | one of [ blocks | txs | receipts | logs | traces | names | when | where | one | two | ls ] |
|  | sub_cmds | sub-command[s] for <cmd>. Use qblock cmd --help for help on the cmd |
| -h | --help | display this help screen |

`Notes:`

- You may add you own commands by entering a value of `add_cmds` to the config file.

