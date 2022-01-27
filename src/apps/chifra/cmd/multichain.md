MAKE A NOTE IN THE DOCS OF WHERE THINGS FALL SHORT
MAKE A NOTE IN DOCS ABOUT WHAT IS AND WHAT IS NOT INDEXED BY US

  - [ ] `chifra explore` needs to be per chain


- [ ] rpc must follow chain to valid RPC (with tracing!)
- [ ] we should force the user to tell us the chain id when the add a new chain and then check it against the RPC we get if it's new and we've never seen it before. Save that info away so we don't have to do the same every command

### Open Questions
----
- [ ] Block Scraper
  - [ ] The function initConfig() in blaze needs attention vis-a-vi paths (we should be able to use the chifra paths directly)
  - [ ] Blaze has command line options to explicitly take the paths
  - [ ] We should be able to remove the 'requires' options (tracing, parity -- balances already removed) in blockScrape.toml

- [ ] Other Tools
  - [ ] Does `ethslurp` work per chain?  The Etherscan API needs to be adjusted per chain
  - [ ] `chifra when` is per chain - can we test this? Would need time stamp from multiple chains

- [ ] Docs
  - [ ] ./docs/content/chifra/configs.md needs work
  - [ ] Must document running against private networks and how one must configure the scraper for smaller chunks

- [ ] Chain specific data: 
  - [ ] prefunds, timestamps, prices, manifest, explorer urls, explorer apis, specials

- [ ] Chain specific data with overlay
  - [ ] names

- [ ] Not chain specific data
  - [ ] abis

- [ ] Chifra Export
  - [ ] Uniswap pricing of reconciliations obviously doesn't work on non-main net.

- [x] Closed Issues
  - [x] `chifra init --chain unknown_chain` should fail gracefully
  - [x] Create a different Pinata account for each chain: gnosis.unchainedindex.io, etc.
  - [x] What about the bytzantium hard fork -- is there block number specific behaviour? (probably -- for ETC for example)
  - [x] What about 'status' on pre-byzantium receipts on non-main net chains?
  - [x] Should be use `v1`? Not sure.
  - [x] Don't really like it, but would allow upgrades.
  - [x] Should there be upgrades? Doesn't `manifest.txt` allow for inifinite upgrades?
  - [x] How does the server handle `--chain` and/or a different default chain? (Just works!)
  - [x] When running against remote gnosis server, our app does not send in the --client parameter (does now)
  - [x] You have to start the server with --client gnosis (or use default but client now also sends)
  - [x] The server does not accept a --chain value. Why?
  - [x] The API must accept --chain
  - [x] The server wants to run against the base configuration (i.e. mainnet -- why???). 
  - [x] What if we wanted to run two servers -- one for each of two different chains? (JUST WORKS!)
  - [x] This is another reason why --chain must be a global option.
  - [x] Otherwise, we would have to run multiple servers. As a result -- we want to disable 
  - [x] the `--chain` option for `chifra serve` (probably other things as well)
  - [x] TrueBlocks and other config file
  - [x] Should it have any per-chain data (probably not) (NO)
  - [x] Need to be able to 'merge' toml files so we can have top-level settings and per-chain overlay (Viper makes this easy)
  - [x] Don't expose IndexPath and CachePath (or any config settings) directly. Hide behind function or use a map<string, string>
  - [x] User provides his/her own chain id. We query the chain at the RPC and verify we're connected to the rigth chain
  - [x] Config items should be able to be read from top-level and overlaid with chain specific config
  - [x] If a new chain is presented, create it under the valid `config`, `cache`, and `index` folders - i.e. establish
  - [x] Test with chifra status --chain shit
  - [x] If the config, cache, or index path does not exist, fail without proceeding if it's not the default otherwise create it
  - [x] If the config, cache, or index path does exist, establish its subfolders
  - [x] Test XDG varables somehow
  - [x] Add test cases for XDG both valid and not valid
  - [x] If the path comes from XDG it must both exist and be fully resolved, otherwise fail
  - [x] If the path contains `cache` or `unchained` or end with one one of the caches or index folders, fail without proceeding (not true -- let the user specify any path they want)
  - [x] The prefund values per chain are in the repo but not being used
  - [x] We need to use the `--chain` option when building paths
  - [x] Rename all path routines from `getXXXPath` to `getPathToXXX` so we can find them more easily
  - [x] Put all the path related functions in a single file to better control them (one for .go, one for .cpp)
  - [x] Concentrate all changes to paths in the golang code, and send environment varaibles (current) or command line options 
  - [x] (if needed) to cary paths to the C++ code.
  - [x] Add `[settings]chain=` option to trueBlocks.toml [No -- we don't want to do this. `--chain` is a command
  - [x] line option only with `main net` the default in absences of that option.]
  - [x] Make sure the three base routines that return paths return paths ending with '/' (dependant routines already do this).
  - [x] Add a global command line option `--chain`
  - [x] Use XDG spec for environment variables
  - [x] Expand performance testing to include chain (set to `main net` for all records)
  - [x] Add chain and network/chainid to meta data
  - [x] We need to fix the install files. XDG is present, but does it work?
  - [x] Make sure blockScrape.toml is in the correct location after install

### Steps to Migrate
---
- [ ] The CheckMigrations routine should fail if the migration isn't complete
- [ ] Some of the `cmake` files and the `make install` may need to be modified.
- [ ] Some of the files and scripts in `build_assets` may have to be re-worked
- [ ] We need to document the way these paths work in the website (search for XDG to find out where)
- [ ] Add `chain` to the `status` endpoints including `--terse`
- [ ] Finish the migrations-0.25.0.md file
- [ ] Re-write the `trueBlocks.toml` file with version `0.25.0`. 
  - [ ] Use this as a marker that the installation is migrated. 
  - [ ] Alternate: write migrations to the file [migrations]0.25.0=true, etc. (not a good idea)

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
  - if empty, set it to default `main net`
  - call this value `CHAIN`

- Where are chain specific configurations?
  - Only two configurations are not chain specific:
    - `trueBlocks.toml` file and 
    - `known` abi files (which remain correct even accross chains)
    - Always find these at `$TB_HOME/`
  - Otherwise chain specific configs are found at `$TB_HOME/config/$CHAIN/`
  - Example: `$TB_HOME/config/main net/blockScrape.toml`
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
- If the version of `trueBlocks.toml` is `0.25.0` or later, we don't need to check any previous migrations
- If the `$TB_HOME` contains any of the following folders, fail.
  - `cache`, `mocked`, `manifest`, `unchained`, `ts.bin.gz`, `poloniex.tar.gz`
  - Tell the user to complete the migration and quit
- If the `$TB_HOME` contains any `toml` file other than `trueBlocks.toml` 
  - Tell them to move them to the chain-specific config and quit
- The installer will install a few config files on first build:
  - If there is no `$TB_HOME/abis` folder, fail
  - If there is no `$TB_HOME/config`, fail
  - If there is no `$TB_HOME/config/main net`, fail
  - If there is no `$TB_HOME/config/main net/manifest/`, fail
  - Would be amazing if the rest of the configuration for a chain could be found in the manifest
  - Alt: If there is no `$TB_HOME/config/main net/names/`, fail
  - Alt: If there is no `$TB_HOME/config/main net/allocs/`, fail

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

| Chain Id | Chain     | Remote Explorer                      | Public RPC | Genesis Hash (from Erigon)                                         |
| -------- | --------- | ------------------------------------ | ---------- | ------------------------------------------------------------------ |
| 1        | mainnet   | https://etherscan.io                 |            | 0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3 |
| 3        | ropsten   | https://ropsten.etherscan.io         |            | 0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d |
| 4        | rinkeby   | https://rinkeby.etherscan.io         |            | 0x6341fd3daf94b748c72ced5a5b26028f2474f5f00d824504e4fa37a75767e177 |
| 5        | goerli    | https://goerli.etherscan.io          |            | 0xbf7e331f7f7c1dd2e05159666b3bf8bc7a8a3a9eb1d518969eab529dd9b88c1a |
| 42       | kovan     | https://kovan.etherscan.io           |            | 0xa3c565fc15c7478862d50ccd6561e3c06b24cc509bf388941c25ea985ce32cb9 |
| 137      | polygon   | https://polygonscan.com              |            | 0xa9c28ce2141b56c474f1dc504bee9b01eb1bd7d1a507580d5519d4437a97de1b |
| 56       | bsc       | https://bscscan.com                  |            | 0x0d21840abff46b96c84b2ac9e10e4f5cdaeb5693cb665db62a2f3b02d2d57b5b |
| 100      | gnosis    | https://blockscout.com/xdai/mainnet/ |            | 0x4f1dd23188aab3a76b463e4af801b52b1248ef073c648cbdc4c9333d3da79756 |
| 61       | etc       | https://blockscout.com/etc/mainnet/  |            | 0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3 |
|          | polkadot? |                                      |            | ???                                                                |
|          | filecoin? |                                      |            | ???                                                                |

### Existing Paths

#### GoLang
----
| Name                 | Description                                                                  |
| -------------------- | ---------------------------------------------------------------------------- |
| GetPathToRootConfig  | Where to find the trueBlocks.toml                                            |
| GetPathToCHainConfig | Where to find the per-chain config files                                     |
| GetPathTo Cache      | Found in config file<br>defaults to `GetPathToChainConfig() + "cache"`       |
| GetPath ToIndex      | Found in config file<br>defaults to `GetPathToChainConfig() + "unchained"`   |
| GetPathToCommands    | Hard coded to $HOME/.local/bin/chifra on both platforms in GetPathToCommands |

<br>

#### C++
----
| Name                | Description                                                                                              |
| ------------------- | -------------------------------------------------------------------------------------------------------- |
| getPathToRootConfig |                                                                                                          |
|                     | get PathToIndex, getPath ToCache                                                                         |
| getPathToCache      | `cachePath` from trueBlocks.toml                                                                         |
|                     | getPathToBinaryCache, getPathToPriceDb, getPathToMonitor,<br/>getPathToMonitorDels, getPathToMonitorLast |
| getPathToIndex      | `indexPath` from trueBlocks.toml                                                                         |
| getPathToCommands   |                                                                                                          |

#### Local Tooling Only
----
| Name               | Description                                             |
| ------------------ | ------------------------------------------------------- |
| getPathToDocs      | For internal tooling only - works from `./build` folder |
| getPathToTemplates | For internal tooling only - works from `./build` folder |
| getPathToSource    | For internal tooling only - works from `./build` folder |
