## qblocks

`qblocks` is the overarching command for all QBlocks tools. Enter `qblocks --help` for more information.

#### Usage

`Usage:`    qblocks [] &lt;cmd&gt; &lt;sub_cmds&gt;  
`Purpose:`  Overarching command for all QBlocks tools.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | cmd | one of [ block | trans | receipt | logs | trace | bloom | accounts | balance | tokenbal | contract | name | when | where |  ] |
|  | sub_cmds | sub-command[s] for <cmd>. Use qblock cmd --help for help on the cmd |

`Notes:`

- You may add you own commands by entering a value in the config file.

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


