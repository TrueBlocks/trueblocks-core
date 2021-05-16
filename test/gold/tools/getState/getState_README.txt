getState argc: 2 [1:-th] 
getState -th 
### Usage

`Usage:`    chifra state [-p|-c|-n|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve account balance(s) for one or more addresses at given block(s).

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | addrs | one or more addresses (0x...) from which to retrieve balances (required) |
|  | blocks | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -p | --parts &lt;val&gt; | control which state to export, one or more of *[ none \| some\* \| all \| balance \| nonce \| code \| storage \| deployed \| accttype ]* |
| -c | --changes | only report a balance when it changes from one block to the next |
| -n | --no_zero | suppress the display of zero balance accounts |
|####|Hidden options||
| -a | --call &lt;str&gt; | a bang-separated string consisting of address!4-byte!bytes |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple `modes` on a single line.

