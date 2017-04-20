# ethslurp
Drink the blockchain (http://ethslurp.com)

Watch a video tutorial <a href="https://www.youtube.com/watch?v=ZZDV1yAgces">here</a>

Check out Great Hill Corporation on <a href="https://www.facebook.com/GreatHillCorporation/">Facebook</a> or <a href="https://twitter.com/ethslurp">Twitter</a>

### ethslurp

`Usage:`    ethslurp [-a|-b|-d|-m|-n|-r|-f|-i|-e|-o|-l|-c|-t|-v|-h] addr  
`Purpose:`  Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | addr | the address of the account or contract to slurp |
| -a | --archive | filename of output (stdout otherwise) |
| -b | --blocks | export records in block range (:0[:max]) |
| -d | --dates | export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]]) |
| -m | --max | maximum transactions to slurp (:250000) |
| -n | --name | name this address |
| -r | --rerun | re-run the most recent slurp |
| -f | --fmt | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -i | --income | include income transactions only |
| -e | --expense | include expenditures only |
| -o | --open | open the configuration file for editing |
| -l | --list | list previously slurped addresses in cache |
| -c | --clear | clear all previously cached slurps |
| -t | --test | generate intermediary files but do not execute the commands |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

  Portions of this software are Powered by Etherscan.io APIs and QuickBlocks.io

### Building from source

Please see the <a href="https://github.com/Great-Hill-Corporation/ethslurp/blob/master/INSTALL.md">INSTALL.md</a> file for more information.
