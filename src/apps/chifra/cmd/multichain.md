### Steps to Migrate
---
- <div style="text-decoration:line-through">Rename all path routines from `getXXXPath` to `getPathToXXX` so we can find them more easily</div>
- <div style="text-decoration:line-through">Put all the PathAccessor functions in a single file to better control them (one for .go, one for .cpp)</div>
- Add `[settings]chain=` option to trueBlocks.toml
- We need to add global command line option `--chain` availble to all tools and overriding any configuration settings.
- Use XDG spec for environment variables
- Expand performance testing to include chain (set to `mainnet` for all records)
- Fix both the `cmake` files and the `make install` if necassary.
- The files and scripts in `build_assets` may have to be re-worked
- We need to document this in the README (search for XDG to find out where)
- Add `chain` to the `status` endpoints
- Add testing to utillib tests to set XDG folders, and try to create non-existant folders
- Finish the migrations-0.24.0.md file
- Make sure all routines that return paths return paths ending with '/'
- When block special blocks are mainnet centric
- Re-write the `trueBlocks.toml` file with version `0.24.0`

### What We Need to Control
---
There are three values we need to control:
- Where is the `trueBlocks.toml` file?
  - `$XDG_DATA_HOME/trueblocks/`, if not empty
  - `$HOME/.local/share/trueblocks/`, if on Linux
  - `$HOME/Library/Application Support/TrueBlocks/`, if on Mac
  - Call this folder `TB_HOME`

- Get the value of `$TB_HOME/trueBlocks.toml:[settings]chain`
  - if not found, create it with default values
  - if missing, add value of `mainnet` and re-write
  - update version when writing out new values
  - call this value `CHAIN`

- Where is configuration
  - `$XDG_CONFIG_HOME/trueblocks/$CHAIN/`, if not empty
  - else the value stored at `trueBlocks.toml:[settings]configPath/$CHAIN/`
  - else `$TB_HOME/config/$CHAIN/`

- Where is cache
  - `$XDG_CACHE_HOME/trueblocks/$CHAIN/`, if not empty
  - else the value stored at `trueBlocks.toml:[settings]cachePath/$CHAIN/`
  - else `TB_HOME/cache/$CHAIN/`

- For all three folders, we create the folder if it doesn't exist
- For the configuration and cache
  - we append the value of trueBlocks.toml [settings]`chain` or `mainnet` if empty

### Rules for checking proper migration in GoLang code
---
- If the version in `trueBlocks.toml` is `0.24.0` or later, no migration needed
- If the `configPath` contains `cache`, `mocked`, `manifest`, `unchained`, `ts.bin.gz`, `poloniex.tar.gz`, tell the user to move it
- If the `configPath` contains any `toml` file other than `trueBlocks.toml`, tell them to move it.
- Auto create if it doesn't exist `configPath/mainnet/config`, folder
- If there is no mainnet folder, fail
- If there is no Abis folder, fail
- If there is no mainnet/config/names.tab, fail
- If there is no mainnet/config/allocs.tab, fail

### Format of new config file
---

```
$BASE_PATH = `~/local/share/trueblocks` or `~/Library/Application Support/TrueBlocks/`

[settings]
chain = ""              # defaults to mainnet (must be one of mainnet, ropstem, rinkeby, goerli, kovan, xdai)
basePath = ""           # defaults to ${BASE_PATH}

[mainnet]
rpcProvider = ""
remoteExplorer = ""

[rinkeby]
rpcProvider = ""
remoteExplorer = ""
```

### Calculated Paths
---
We want the only configuration to be ${BASE_PATH}. All other paths are calculated based on that value and ${CHAIN}

| Name      | Calculated Value                |
| --------- | ------------------------------- |
| cachePath | {BASE_CACHE}/cache/{CHAIN}/     |
| indexPath | {BASE_CACHE}/unchained/{CHAIN}/ |

### Per-Chain Data

| Chain Id | Chain     | Remote Explorer              | Erigon      | Genesis Hash (from Erigon)                                         |
| -------- | --------- | ---------------------------- | ----------- | ------------------------------------------------------------------ |
| 1        | mainnet   | https://etherscan.io         | mainnet     | 0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3 |
| 3        | ropsten   | https://ropsten.etherscan.io | reposten    | 0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d |
| 4        | rinkeby   | https://rinkeby.etherscan.io | rinkeby     | 0x6341fd3daf94b748c72ced5a5b26028f2474f5f00d824504e4fa37a75767e177 |
| 5        | goerli    | https://goerli.etherscan.io  | goerli      | 0xbf7e331f7f7c1dd2e05159666b3bf8bc7a8a3a9eb1d518969eab529dd9b88c1a |
| 42       | kovan     | https://kovan.etherscan.io   | kovan       | 0xa3c565fc15c7478862d50ccd6561e3c06b24cc509bf388941c25ea985ce32cb9 |
|          |           | https://polygonscan.com      |             |                                                                    |
| 56       |           | https://bscscan.com          | bsc-mainnet | 0x0d21840abff46b96c84b2ac9e10e4f5cdaeb5693cb665db62a2f3b02d2d57b5b |
|          |           | https://hecoinfo.com         |             |                                                                    |
|          |           |                              | erigonmine  | 0xfecd5c85712e36f30f09ba3a42386b42c46b5ba5395a4246b952e655f9aa0f58 |
| 1212120  |           |                              | fermion     | 0x0658360d8680ead416900a552b67b84e6d575c7f0ecab3dbe42406f9f8c34c35 |
| 77       |           |                              | sokol       | 0x5b28c1bfd3a15230c9a46b399cd0f9a6920d432e85381cc6a140b06e8410112f |
|          |           |                              | yolov3      |                                                                    |
| 97       |           |                              | chapel?     | 0x6d3c66c5357ec91d5c43af47e234a939b22557cbb552dc45bebbceeed90fbe34 |
| 1417     |           |                              | rialto?     | 0x005dc005bddd1967de6187c1c23be801eb7abdd80cebcc24f341b727b70311d6 |
|          | xdai      |                              |             | 0x4f1dd23188aab3a76b463e4af801b52b1248ef073c648cbdc4c9333d3da79756 |
|          | polkadot? |                              |             | ???                                                                |
|          | filecoin? |                              |             | ???                                                                |

### Open Questions
----
- What about bytzantium and the 'status' value for non-mainnet chains?
- What about xDai?

### Existing Paths

<br>

#### Go Lang Paths
----
| Name              | Description                                                                                                      |
| ----------------- | ---------------------------------------------------------------------------------------------------------------- |
| GetPathToConfig   | Hard coded in utils.go<br>`~/local/share/trueblocks` (Linux)<br>`~/Library/Application Support/TrueBlocks` (Mac) |
| GetPathToCache    | Found in config file<br>defaults to `GetPathToConfig() + "cache"`                                                |
| GetPathToIndex    | Found in config file<br>defaults to `GetPathToConfig() + "unchained"`                                            |
| GetPathToCommands | Hard coded to $HOME/.local/bin/chifra on both platforms in GetPathToCommands                                     |

<br>

#### C++ Paths
----
| Name              | Description                                                                                              |
| ----------------- | -------------------------------------------------------------------------------------------------------- |
| getPathToConfig   |                                                                                                          |
|                   | getPathToIndex, getPathToCache                                                                           |
| getPathToCache    | `cachePath` from trueBlocks.toml                                                                         |
|                   | getPathToBinaryCache, getPathToPriceDb, getPathToMonitor,<br/>getPathToMonitorDels, getPathToMonitorLast |
| getPathToIndex    | `indexPath` from trueBlocks.toml                                                                         |
| getPathToCommands |                                                                                                          |



<br>

#### Local Tooling Only
----
| Name               | Description                                             |
| ------------------ | ------------------------------------------------------- |
| getPathToDocs      | For internal tooling only - works from `./build` folder |
| getPathToTemplates | For internal tooling only - works from `./build` folder |
| getPathToSource    | For internal tooling only - works from `./build` folder |

Search for IndexPath, CachePath in golang code
Blaze has command line options to explicitly take the paths
We used to preclude the user from customizing an indexPath if it doesn't exist
Should be fail if the XDG paths do not exist?


-------
We should be able to remove the 'requires' options (tracing, parity, balances) in blockScrape.toml
Names and timestamps are mainnet centric
Poloniex prices are mainnet centric
The unchained index needs chain id
The make process installs files in the default location but doesn't really work with XDG or customized folders in the config file
The prefund values per chain are in the repo but not being used
Should we try to support chain agnostic chain ids?
Should we check that we are running against the chain the user is telling us we are running against?
Manifest files are chain centric
All other caches (other than perhaps the abis) are also chain centric
EtherScan APIs will be chain specific
Private networks have a real problem with 'front of chain' because almost all of the thing is front of chain
Uniswap pricing of reconciliations obviously doesn't work on non-mainnet.
Test ABI generation for non-mainnet chains
Check out https://github.com/symblox/hardhat-abi-gen for ABI generation from Solidity code
