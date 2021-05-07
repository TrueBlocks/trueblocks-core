## chifra explore

`chifra explore` opens Etherscan (and later other explorers - including our own) to the block, transaction hash, or Ethereum address you specify. It's a handy way to open EtherScan from the command line, nothing more.

### usage

`Usage:`    chifra explore [ address | tx_hash | block_num ]  
`Purpose:`  Opens Etherscan to either an address, a transaction hash, or a block number.

`Where:`  

| Hotkey | Option | Description |
| -------: | :------- | :------- |
|  | addrs | an Ethereum address |
|  | tx_hash | a 32-byte transaction hash |
|  | block_num | a block number |
| -h | --help | display this help screen |

`Notes:`

- An `addresses` must start with '0x' and be forty-two characters long.
- A `tx_hash` must start with '0x' and be sixty-six characters long.
- If a hash is provided, it is assumed to be a transaction hash.
