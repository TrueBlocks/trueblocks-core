`chifra {{.Route}}` is the first tool we built in the Ethereum space. It even has its [own website](http://ethslurp.com).

While it's useful, it has two shortcomings. First, it is fully centralized, pulling its data from
[http://etherscan.io](http://etherscan.io). Second, is that it does not report every transaction
for a given account. This is actually a shortcoming with API providers. It's too complicated to explain
here, but see our blog.

While `chifra {{.Route}}` has its shortcomings, it does provides some nice features. You may use it to pull
any transaction initiated by an EOA for example or to explore mining rewards. Visit the above
referenced website for more information.

Currently supported API providers:
- [TrueBlocks Key](https://key.trueblocks.io)
- Etherscan
- Covalent
- Alchemy

### Adding provider API key
Call `chifra config edit` to edit the configuration file.

For TrueBlocks Key, add `keyEndpoint = "your-key-endpoint-url"` to `chains.mainnet` section.

For all other providers add an entry to `keys` section like this:
```toml
[keys]
  [keys.etherscan]
    apiKey = "etherscan-apikey"
  [keys.covalent]
    apiKey = "covalent-apikey"
  [keys.alchemy]
    apiKey = "alchemy-apikey"
```