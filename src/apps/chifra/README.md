## chifra

<hr>
**Note:** This tool is availble through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
<hr>

`chifra` is an command line tool that builds [QuickBlocks account monitors](../../monitors/README.md) through a series of interactive questions.

Given your answers to those questions (such as which Ethereum addresses to monitor, what to name those addresses, where to find ABI files for those addresses), `chifra` generates the C++ code for both the parsing library needed to monitor the addresses and the code to build an interative transactional debugging/auditing program.

Prior to running the program, collect together three pieces of information for each address you wish to monitor. You may monitor as many addresses as you wish. The information you need per address is:

- an Ethereum address
- a name to assign to this address (you may pick up the name using [ethName](../../tools/ethName/README.md))
- the block number when this address was first used, or (if a smart contract) when it was deployed.

`Chifra` uses [makeClass](../makeClass/README.md) and [grabABI](../grabABI/README.md) to build the monitors.

Here is a screen shot of an interaction with Chifra building a smart contract monitor for a particular token.

<img src=docs/image.png>

#### Usage

`Usage:`    chifra [-s|-v|-h] folder addrs  
`Purpose:`  Interactively creates a QuickBlocks project to monitor an Ethereum address.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | folder | name of the folder into which to store the monitor |
|  | addrs | a list of one or more addresses to monitor (must start with '0x') |
| -s | --silent | suppress all output from chifra (normally chifra is quite verbose) |
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


