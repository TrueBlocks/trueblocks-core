chifra argc: 2 [1:-th] 
chifra -th 
#### Usage

`Usage:`    chifra [-s|-v|-h] folder &lt;address&gt; [address...]  
`Purpose:`  Interactively creates a QBlocks monitor for the given address.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | folder | name of the monitor (also the ./folder for the source code) |
|  | address_list | a list of one or more addresses to monitor (must start with '0x') |
| -s | --silent | suppress all output from chifra (normally chifra is quite verbose) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty characters long.

