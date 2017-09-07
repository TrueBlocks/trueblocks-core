## exportContext

Testing only

my string: my string : 24
bool: 1 : 1
char: c : 1
unsigned int (this is a bug, should be 4294967295): 4294967295 : 4
int: -1 : 4
float: 10.3330001831 : 4
double: 1001001010.0112111568 : 8
time: 2017-10-12 01:14:12 UTC : 8
int64_t: -1 : 8
uint64_t: 18446744073709551615 : 8

my string: my string : 24
bool: 1 : 1
char: c : 1
unsigned int: 4294967295 : 4
int: -1 : 4
float: 10.3330001831 : 4
double: 1001001010.0112111568 : 8
time: 2017-10-12 01:14:12 UTC : 8
int64_t: -1 : 8
uint64_t: 18446744073709551615 : 8
#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |         Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. Use semi-colon make comments.*

**Powered by QuickBlocks<sup>&reg;</sup>**


