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