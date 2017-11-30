ethName argc: 2 [1:-th] 
ethName -th 
#### Usage

`Usage:`    ethName [-a|-c|-d|-o|-l|-m|-s|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query Ethereum addresses and/or names making it easy to remember accounts.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | terms | a space separated list of one or more search terms |
| -a | --addr | export only the associated address (may be used in scripting) |
| -c | --count | print only the count of the number of matches |
| -d | --data | export results as tab separated data |
| -o | --open | open the name database for editing |
| -l | --list | list all names in the database |
| -m | --matchCase | matches must agree in case (the default is to ignore case) |
| -s | --source | search 'source' field as well name and address (the default) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- With a single search term, the tool searches both `name` and `address`.
- With two search terms, the first term must match the `address` field, and the second term must match the `name` field.
- When there are two search terms, both must match.
- If one mixes options, the `--edit` option predominates (i.e. the program opens the database and then quits).
- The `--list` option predominates otherwise. If present, the tool displays a list of stored names and addresses and then quits.
- The `--count` option works with any other option and will simply display the number of matches.
- The `--matchCase` option requires case sensitive matching. It works with all other options.
- The `--addrOnly` option modifies the display output and therefore works with any other options.

