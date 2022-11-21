<!-- markdownlint-disable MD024 -->
# Changes

This file details changes made to TrueBlocks per version (starting with version v0.40.0). See the [migration notes](./MIGRATIONS.md) for changes from previous versions.

## v0.43.0

### System-Wide Changes

- Bump version to v0.43.0
- Better support for `--file`, `--to_file`, and `--output` options for all tools.
- Use of improved `StreamMany` routines to allow for concurrent rendering of data structures in GoLang code (thanks Dawid)
- Large number of addition test cases for all tools. Better debugging tools and testing environment.
- *Breaking change:* removes `--dollars` option from all tools. (Users may use `spot_price` value where available.)

### Changes to Specific Tools

- **chifra export:**
  - Improvements to accounting module. Much better support for token accounting including multiple token transfers in a single transaction and/or block.
  - *Breaking change:* change to `--statements` option which is now only available under the `--accounting` as per user suggestion.
  - *Breaking change:* `--accounting` no longer turns `--articulate` on by default (users must explicitly add `--articulate` to get same behaviour).
  - CStatement class renamed to CLedgerEntry to improve clarity of code.
  - Adds `-f` hotkey to `--flow` option.

- **chifra monitors:**
  - Adds `--decache` and `--list` options to help clean up caches.

- **chifra state:**
  - *Breaking change:* Remove `--storage` option (this option was never fully implemented anyway).
  - Add `--proxy` option which allows user to specify the address of a proxied-to address. Useful with the `--call` option.
  - Improvements to the `chifra state --call` function to recongnize and automatically call proxied-to contracts (thanks to contributor Matt Solomon)

- **chifra transactions:**
  - *Breaking change:* deprecates `--reconcile` option, replaces it with `--account_for` option which works identically.

- **chifra when:**
  - Add `--update` to explicitly update the timestamp database (as opposed to implicit update depending on context).
  - Unhides `--repair` and `--check` options.

## v0.41.0

### System-Wide Changes

- *Breaking change:* removes `--fmt api` option from all tools. (Users must change to `--fmt json` instead.)
- Better support for `--file`, `--to_file`, and `--output` options for all tools.
- Improvements to `--fmt` options to better support customized display strings in the near future.
- Better support for overriding command line options with environment variables.

### Changes to Specific Tools

- **chifra export:**
  - Adds `-f` hotkey to `--flow` option.

- **chifra serve:**
  - Fixes issues related to using the `export` route through the Api and the `maxRecords` and `cache` options.

- **chifra blocks:**
  - Adds `--flow [from|to|reward]` option to show only blocks with the given address in the given location.

- **chifra transactions:**
  - Adds `--flow [from|to]` option to show only transactions with the given address in the given location.

- **chifra explore:**
  - Switched default explorer for gnosis chain to [https://gnosisscan.io/](https://gnosisscan.io/).

- **chifra quotes:**
  - *Breaking change:* This tool has been fully deprecated. It is no longer available.

### Changes to Data Models

- **Logs:**
  - Added `blockNumber`, `transactionIndex`, and `transactionHash` to the Log data structure when exported by `chifra logs`, `chifra transactions`, and `chifra export --logs`

- **Trace:**
  - Added `timestamp` to the Trace data structure as exported by `chifra traces`, `chifra transactions --traces`, and `chifra export --traces`

- **Name:**
  - Removed `description` field and replaced with an auto-generated `petname` field. (Petnames are described [here](http://www.erights.org/elib/capability/pnml.html).)

### Changes to Config Files

- **acctExport.toml:**
  - Removes `max_slow_queries` option as it was not working anyway.

## v0.40.0

### System-Wide Changes

- Completed the first draft of the [Unchained Index Spec](https://trueblocks.io/papers/2022/file-format-spec-v0.40.0-beta.pdf).
- Re-wrote the Unchained Index smart contract (see below).
- Completely re-wrote the `chifra scrape` tool (see below).
- Added and greatly expanded `chifra chunks` to manage, update, pin, and clean the Unchained Index.
- Enabled both local pinning (if IPFS daemon is running) and remote pinning to various pinning services (in addition to the already support Pinata).
- Added support for (and committed to indexing) Gnosis and Sepolia testnet chains. Wrote detailed instructions for [Indexing Your Own Chain](https://trueblocks.io/docs/prologue/multi-chain/#adding-your-own-chain).
- Removes now-retired testnet chains including Kovan, Rinkeby, and Ropsten.
- Preparation for publishing and better documenting GoLang packages for support of future application level code.
- Preparation for eventual removal of all C++ code.
- Updated and expanded many test cases.
- Modified a few configuration options in `trueBlocks.toml` file:
  - Added `[settings]defaultGateway` option for support of `--pin --remote` options in various tools.
  - Replaced `pinGateway` chain-specific options with `ipfsGateway`.
  - Added `[keys]` section to support multiple API keys including keys for pinning services
  - Moved `[settings]etherscan_key` to `[keys.etherscan]apiKey`.

### Changes to Specific Tools

- **Unchained Index Smart Contract:**
  - to make it more broadly useful (for not only ourselves, but for anyone who chooses to use it). Previously, the contract allowed publication from a single known address (ours) and only accepted a single IPFS hash. With this release, we've chosen to allow any publisher to publish any IPFS hash for any reason. (In the case of our software, we know that we're interested in the hashes that we publish, so this is no less secure than the previous smart contract.) The smart contract is much better explained in the [Unchained Index Spec](https://trueblocks.io/papers/2022/file-format-spec-v0.40.0-beta.pdf).

- **chifra scrape:**
  - Removed all C++ code from the scraper allowing much better GoLang concurrency resulting in 2x speedup of scraper.
  - Now writes the Sha256 hash of the Unchained Index Spec into each index header as a versioning mechanism.
  - Adds versioning to the index files and blooms which allows for auto-correction and future migrations.
  - Allows for downloading databases for timestamps and names from the smart contract.
  - Added `--first_block` option (enabled but not implemented).
  - Added `--sleep` option to better respond to slower node endpoints.
  - Added `--remote` option to `--pin` allowing for pinning locally as well as to an arbitrary pinning service.
  - Makes `--block_cnt` a command-line only option (removes ability to use configuration file setting).
  - Removed `mode` option which means user no longer needs to specify `run` sub-command.
  - Prepares the code for much better documentation including auto-generation of GoDocs code for packages.
  - Disallows running against non-archive, non-tracing nodes (including Geth).
  - Removes configuration items for `[settings]block_chan_cnt` and `[settings]addr_chan_cnt` replacing both with `[settings]channel_count`
  - Removes configuration items for:
    - `[settings]n_blocks` (not used),
    - `[settings]n_test_runs` (not used),
    - `[settings]pinata_api_key` and `[settings]pinata_secret_api_key` (moved to `trueBlocks.toml` under the `[pins.pinata]` section),
    - `[requires]tracing`, `[requires]archive`, and `[requires]parity` (no longer supported),
    - `[dev]ipfs_gateway` (moved to `trueBlocks.toml` under the `[settings]defaultGateway` and per-chain sections),
  - Converted the manifest to JSON to support structured data.
  - Much more robust error handling.

- **chifra export:**
  - Added new command line option, `--flow`, which allows for exporting `[ in | out | zero ]` transactions (that is, incoming value, outgoing value, or zero value transactions).
  - Removes `--staging` option since it is no longer needed as `chifra list` now includes staged transactions by default (see below).
  - Fixed a bug related to the `--neighbors` option ignoring `--first_block` option.

- **chifra list:**
  - Expanded which transactions `chifra list` considers for inclusion in a monitor to include "staged but not consolidated" transactions. That is, transactions that are older than 28 blocks but not yet consolidated into an index chunk (see `unripe_dist` above).
  - This change allows `chirfra export` to display transactions 28 blocks old or older (about six minutes). 
  - Previously, only consolidated transactions were reported (about seven hours behind the head, on average).
  - You may use the `--unripe` option of `chifra export` to see transactions less than 28 blocks old, but use this data with caution due to re-orgs.

- **chifra init:**
  - `chifra init --all` now updates the locally-stored index to mirror the index as reported by the Unchained Index manifest (for files older than the manifest).
  - Re-wrote the entire tool to make it much more robust:
    - Much better error handling ensures easier to reproduce indexes,
    - Allows for progressive backing off of requests if the pinning service is rate limiting,
    - Downloads both index chunk and bloom filter "as a whole" to ensure better consistency of the download.
  - Added `--sleep` option to slow down the download to better avoid pinning service timeouts (the option is allowed, but currently a `noop`).
  - Added `--first_block` option to allow partial download of the index (the option is allowed, but currently a `noop`).

- **chifra chunks:**
  - Completely re-wrote this tool to greatly expand its capabilities including:
    - Much better display of the chunk data with the `index`, `blooms`, `manifest`, `addresses`, `appearances`, `stats`, and `status` modes.
    - Both `addresses` and `appearances` mode were expanded and improved.
    - Expanded the `--check` option to verify seven different aspects of the index for correctness.
    - Added the `--pin` and `--remote` options to publish the index to either the local IPFS daemon or a remote IPFS pinning service.
    - Added `--publish` option (currently a `noop`).
    - Added `--truncate` option to remove any files in the index after (and including) the given block (***use with caution***).
    - Added `--sleep` option (available on to `--pin --remote`) to "slow down" the upload to avoid time outs.
  - Removed 
    - `pins` mode (not needed)
    - `addresses` argument (not needed, replaced with `--belongs`)
    - `details` option (use `--verbose` instead)
    - `--clean` option (accomplish the same things with `chifra init --all`)
    - `--pin_remote` option (replaced with `--remote`)
      -s, --sleep float       for --remote pinning only, seconds to sleep between API calls

- **chifra monitors:**
  - Changed the short code for the `--sleep` option from `-E` to `-s`.

- **chifra when:**
  - Replaces the `--reset <blk_num>` option with `--truncate <blk_num>` for clarity.
  - Added `--repair` option (this is allowed, but it currently a `noop`).

- **chifra abis:**
  - Added `--hint` option to allow specifying a shortcut to the `--find` option

- **chifra slurp:**
  - This tool will eventually be deprecated.
  - Explicitly removed support for any chain other than Ethereum mainnet.

- **chifra qoutes:**
  - Officially deprecated
