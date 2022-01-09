### Steps to Migrate
---
- [ ] The CheckMigrations routine should fail if the migration isn't complete
- [ ] We need to use the `--chain` option when building paths
- [ ] Some of the `cmake` files and the `make install` may need to be modified.
- [ ] Some of the files and scripts in `build_assets` may have to be re-worked
- [ ] We need to document the way these paths work in the website (search for XDG to find out where)
- [ ] Add `chain` to the `status` endpoints including `--terse`
- [ ] Add testing to utillib tests to set XDG folders using test case .env files
- [ ] Add testing that specifies non-existant folders
- [ ] Finish the migrations-0.24.0.md file
- [ ] Re-write the `trueBlocks.toml` file with version `0.24.0`. Use this as a marker that the installation is migrated. Alternate: write migrations to the file [migrations]0.24.0=true, etc. (this is the same thing - it's easier to write the version)

### Steps to Migrate (completed)
---
- [x] Rename all path routines from `getXXXPath` to `getPathToXXX` so we can find them more easily
- [x] Put all the PathAccessor functions in a single file to better control them (one for .go, one for .cpp)
- [x] Concentrate all changes to paths in the golang code, and send environment varaibles (current) or command line options (if needed) to cary paths to the C++ code.
- [x] Add `[settings]chain=` option to trueBlocks.toml [No -- we don't want to do this. `--chain` is a command line option only with `mainnet` the default in absences of that option.]
- [x] Make sure the three base routines that return paths return paths ending with '/' (dependant routines already do this).
- [x] Add a global command line option `--chain`
- [x] Use XDG spec for environment variables
- [x] Expand performance testing to include chain (set to `mainnet` for all records)

### What We Need to Control
---
There are three values we need to control:

- Where is the `trueBlocks.toml` file?
  - `$XDG_CONFIG_HOME/trueblocks/trueBlocks.toml`, if $XDG_CONFIG_HOME is not empty
  - `$HOME/.local/share/trueblocks/trueBlocks.toml`, on Linux
  - `$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml`, on Mac
  - Call this folder `TB_HOME`

- How do we get the value of `chain`?
  - Command line option only (this is already there)
  - Value is sent to C++ code in an environment variable
  - if empty, set it to default `mainnet`
  - call this value `CHAIN`

- Where are chain specific configurations?
  - Only two configurations are not chain specific:
    - `trueBlocks.toml` file and 
    - `known` abi files (which remain correct even accross chains)
    - Always find these at `$TB_HOME/`
  - Otherwise chain specific configs are found at `$TB_HOME/config/$CHAIN/`
  - Example: `$TB_HOME/config/mainnet/blockScrape.toml`
  - Users may place a config file at `$TB_HOME`. If found, it will be read first and chain-specific values will be 'overlayed' if present or appended if not.

- Where are the chain-specific caches?
  - All caches are chain specific
  - If there is a value at `[settings]cachePath=` in the `trueBlocks.toml`
    - Use it
    - if the value does not end with `/cache/$CHAIN`
      - Remove `cache` if present
      - Append `cache/$CHAIN`
  - If the value of `cachePath` is empty
    - Set `$CACHE_PATH` to `$TB_HOME/cache/$CHAIN/`
  - Create `$CACHE_PATH` if it does not exist
  - Invariant: 
    - `$CACHE_PATH` exists
    - `$CACHE_PATH` ends with `cache/$CHAIN`
  - If above is true, establish all the subcaches

- Where is the unchained index (all indexes are chain-specific)
  - Identically calculated to `cachePath`, but exchange `cache` for `unchained`.

- For all of the above folders, create the folder if it doesn't exist

### Rules for checking proper migration in GoLang code
---
- If the version of `trueBlocks.toml` is `0.24.0` or later, we don't need to check any previous migrations
- If the `$TB_HOME` contains any of the following folders, fail.
  - `cache`, `mocked`, `manifest`, `unchained`, `ts.bin.gz`, `poloniex.tar.gz`
  - Tell the user to complete the migration and quit
- If the `$TB_HOME` contains any `toml` file other than `trueBlocks.toml` 
  - Tell them to move them to the chain-specific config and quit
- The installer will install a few config files on first build:
  - If there is no `$TB_HOME/abis` folder, fail
  - If there is no `$TB_HOME/config`, fail
  - If there is no `$TB_HOME/config/mainnet`, fail
  - If there is no `$TB_HOME/config/mainnet/manifest/`, fail
  - Would be amazing if the rest of the configuration for a chain could be found in the manifest
  - Alt: If there is no `$TB_HOME/config/mainnet/names/`, fail
  - Alt: If there is no `$TB_HOME/config/mainnet/allocs/`, fail

### Format of new config file
---

```
[settings]
cachePath = ""           # defaults to $TB_HOME
etherscan_key = ""

[mainnet]
rpcProvider = "http://localhost:8545"
remoteExplorer = "http://etherscan.io"

[rinkeby]
rpcProvider = ""
remoteExplorer = ""
```

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
|          | gnosis    |                              |             | 0x4f1dd23188aab3a76b463e4af801b52b1248ef073c648cbdc4c9333d3da79756 |
|          | polkadot? |                              |             | ???                                                                |
|          | filecoin? |                              |             | ???                                                                |

### Existing Paths

#### GoLang
----
| Name              | Description                                                                                                      |
| ----------------- | ---------------------------------------------------------------------------------------------------------------- |
| GetPathToConfig   | Hard coded in utils.go<br>`~/local/share/trueblocks` (Linux)<br>`~/Library/Application Support/TrueBlocks` (Mac) |
| GetPathToCache    | Found in config file<br>defaults to `GetPathToConfig() + "cache"`                                                |
| GetPathToIndex    | Found in config file<br>defaults to `GetPathToConfig() + "unchained"`                                            |
| GetPathToCommands | Hard coded to $HOME/.local/bin/chifra on both platforms in GetPathToCommands                                     |

<br>

#### C++
----
| Name              | Description                                                                                              |
| ----------------- | -------------------------------------------------------------------------------------------------------- |
| getPathToConfig   |                                                                                                          |
|                   | getPathToIndex, getPathToCache                                                                           |
| getPathToCache    | `cachePath` from trueBlocks.toml                                                                         |
|                   | getPathToBinaryCache, getPathToPriceDb, getPathToMonitor,<br/>getPathToMonitorDels, getPathToMonitorLast |
| getPathToIndex    | `indexPath` from trueBlocks.toml                                                                         |
| getPathToCommands |                                                                                                          |

#### Local Tooling Only
----
| Name               | Description                                             |
| ------------------ | ------------------------------------------------------- |
| getPathToDocs      | For internal tooling only - works from `./build` folder |
| getPathToTemplates | For internal tooling only - works from `./build` folder |
| getPathToSource    | For internal tooling only - works from `./build` folder |

### Open Questions
----
- What about the bytzantium hard fork -- block number specific behaviour?
- What about 'status' on pre-byzantium receipts on non-mainnet chains?
- Never call it xDAI. Call it gnosos
- We should be able to remove the 'requires' options (tracing, parity -- balances already removed) in blockScrape.toml
- Chain specific data:
  - Names
  - Timestamps
  - Poloniex prices
  - Manifest files
  - EtherScan APIs
  - All caches
  - The unchained index smart contract needs chain id
  - `chifra --chain xdai explore` should go to a different server
  - `chifra slurp` won't work with --chain
  - `chifra when --list` is mainnet centric
- We need to fix the install files. XDG is present, but does it work?
- The prefund values per chain are in the repo but not being used
- Can we use chain ids to verify that the RPC points to the same chain we are being told it is?
- Private networks have a real problem with 'front of chain' because almost all of the thing is front of chain
- Uniswap pricing of reconciliations obviously doesn't work on non-mainnet.
- Test ABI generation for non-mainnet chains
- Check out https://github.com/symblox/hardhat-abi-gen for ABI generation from Solidity code
- The server wants to run against the base configuration (i.e. mainnet). This is another reason why --chain must be a global option. Otherwise, we would have to run multiple servers. As a result -- we want to disable the `--chain` option for `chifra serve` (probably other things as well)
- Search for IndexPath, CachePath in golang code
- Blaze has command line options to explicitly take the paths
- We used to preclude the user from customizing an indexPath if it doesn't exist
- Should be fail if the XDG paths do not exist?
- This function: func initConfig() in blaze needs attention for paths
- ./docs/content/chifra/configs.md needs work
- I feel like I need a different pinata account for each chain. Not a problem. Easier to remove a chain