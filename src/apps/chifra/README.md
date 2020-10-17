## chifra

---

**Note:** This tool is available through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.

---

`chifra` is an command line tool that builds [TrueBlocks account monitors](../../monitors/README.md) through a series of interactive questions.

Given your answers to those questions (such as which Ethereum addresses to monitor, what to name those addresses, where to find ABI files for those addresses), `chifra` generates the C++ code for both the parsing library needed to monitor the addresses and the code to build an interactive transactional debugging/auditing program.

Prior to running the program, collect together three pieces of information for each address you wish to monitor. You may monitor as many addresses as you wish. The information you need per address is:

- an Ethereum address
- a name to assign to this address (you may pick up the name using [ethName](../../tools/ethName/README.md))
- the block number when this address was first used, or (if a smart contract) when it was deployed.

`Chifra` uses [makeClass](../makeClass/README.md) and [grabABI](../grabABI/README.md) to build the monitors.

Here is a screen shot of an interaction with Chifra building a smart contract monitor for a particular token.

<img src=docs/image.png>

#### Usage

`Usage:` chifra [-s|-v|-h] commands  
`Purpose:` Main TrueBlocks command line controls.

`Where:`

| Short Cut | Option        | Description                                                                                                                                                                                                                                                                           |
| --------: | :------------ | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
|           | commands      | which command to run, one or more of [list&#124;export&#124;slurp&#124;names&#124;abi&#124;state&#124;tokens&#124;when&#124;data&#124;blocks&#124;transactions&#124;receipts&#124;logs&#124;traces&#124;quotes&#124;scrape&#124;status&#124;settings&#124;rm&#124;message] (required) |
|        -s | --sleep <num> | for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 14)                                                                                                                                                                           |
|        -v | --verbose     | set verbose level. Either -v, --verbose or -v:n where 'n' is level                                                                                                                                                                                                                    |
|        -h | --help        | display this help screen                                                                                                                                                                                                                                                              |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>_For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment._</small>

**Powered by TrueBlocks<sup>&trade;</sup>**
