- [ ] Docs
  - [ ] ./docs/content/chifra/configs.md needs work
  - [ ] Must document running against private networks and how one must configure the scraper for smaller chunks
  - [ ] Make a note in the docs of where things fall short - uniswap, no index other than a,b,c chains
  - [ ] Make a note in docs about what is and what is not indexed by us
  - [ ] We need to document the way these paths work in the website (search for XDG to find out where)

### Steps to Migrate
---
- [x] The CheckMigrations routine should fail if the migration isn't complete
- [x] Some of the `cmake` files and the `make install` may need to be modified.
- [x] Some of the files and scripts in `build_assets` may have to be re-worked
- [x] Add `chain` to the `status` endpoints
- [x] Finish the migrations-0.25.0.md file
- [x] Re-write the `trueBlocks.toml` file with version `0.25.0`. 
  - [ ] Use this as one of the markers that the installation is completed. 

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
  - If there is no `$TB_HOME/config/mainnet`, fail
  - If there is no `$TB_HOME/config/mainnet/manifest/`, fail
  - Would be amazing if the rest of the configuration for a chain could be found in the manifest
  - Alt: If there is no `$TB_HOME/config/mainnet/names/`, fail
  - Alt: If there is no `$TB_HOME/config/mainnet/allocs/`, fail
