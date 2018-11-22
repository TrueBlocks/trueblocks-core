qblocks argc: 2 [1:-th] 
qblocks -th 
#### Usage

`Usage:`    qblocks [-i] &lt;cmd&gt; &lt;options&gt;  
`Purpose:`  Overarching command for all QBlocks tools.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | cmd | one of [block|trans|receipt|logs|trace|bloom|accounts|balance|tokeninfo|contract|slurp|grab|price|name|when|where] |
|  | options | options sent to <cmd>. Use 'qblock cmd --help' for help on <cmd> |
| -i | --info | Ignore <cmd> and display info about the QBlocks system |

`Notes:`

- You may add you own commands by entering a value in the config file.

