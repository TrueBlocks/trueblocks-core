## miniBlocks

<hr>
**Note:** This tool is availble through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
<hr>

`miniBlocks` walks the QuickBlocks binary cache building highly compacted database which provides significantly faster speed of access to the data.

The tool is intended primarily as a demonstration of how fast QuickBlocks can be depending on what data one is interesting in. The source code for this tool is well documented and may serve as a teaching aid while learning to program with QuickBlocks.
#### Usage

`Usage:`    miniBlocks [-c|-l|-f|-r|-d|-v|-h] mode  
`Purpose:`  Freshen or check the miniBlock database.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode | one of 'check,' 'list,' 'freshen,' or 'refreshen' |
| -c | --check val | check that empty blocks are empty, and visa versa. Optionally start at :block |
| -l | --list | list all non-empty block numbers |
| -f | --freshen | starting from the most recently visited block, append any new, non-empty block numbers to the index |
| -r | --refreshen | remove the full-block index and re-create it |
| -d | --deep | when doing 'check', do a deep check |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |


#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


