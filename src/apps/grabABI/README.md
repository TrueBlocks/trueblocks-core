## grabABI

`grabABI` retreives an ABI file either from a local cache, from the Ethereum Name Service (ENS), or from [Etherscan](http://etherscan.io). Once retreived, `grabABI` generates the classDefinition files needed by [makeClass](../makeClass) to generate a parselib for each [Ethereum address monitor](../../monitors).

#### Usage

`Usage:`    grabABI [-c|-g|-e|-n|-o|-h] addr  
`Purpose:`  Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr | the address(es) of the smart contract(s) to grab |
| -c | --canonical | convert all types to their canonical represenation and remove all spaces from display |
| -g | --generate | generate C++ code into the current folder for all functions and events found in the ABI |
| -e | --encode | generate the encodings for the functions / events in the ABI |
| -n | --noconst | generate encodings for non-constant functions and events only (always true when generating) |
| -o | --open | open the ABI file for editing, download if not already present |
| -h | --help | display this help screen |


** Notes: **

There is also an option`--silent` that is useful for scripting. It causes the app to display fewer messages.
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


