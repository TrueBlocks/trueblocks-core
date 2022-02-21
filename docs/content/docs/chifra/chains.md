BOGUS
- [ ] ./docs/content/chifra/configs.md needs work
- [ ] Must document running against private networks and how one must configure the scraper for smaller chunks
- [ ] Make a note in the docs of where things fall short - uniswap, no index other than a,b,c chains
- [ ] Make a note in docs about what is and what is not indexed by us
- [ ] We need to document the way these paths work in the website (search for XDG to find out where)

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
