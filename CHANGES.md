Made sure RPC and Raw data agrees
Remove transactionLogIndex from Log

Added --sleep and --per_page to chifra slurp
Ported slurp to GoLang
Removes zero-valued Nonce, MaxFeePerGas, and MaxPriorityFeePerGas from Transaction and Block data



<!-- markdownlint-disable MD024 MD036 -->
# Changes

This file details changes made to TrueBlocks per version. See the [migration notes](./MIGRATIONS.md) for changes from previous versions.

## v0.60.0 (2023/03/11)

This is a major release focusing mostly on the continued porting to GoLang. Many of the tools (`chifra abis`, `chifra blocks`, `chifra transactions`, `chifra receipts`, etc.) are now either fully ported to GoLang or well on the way to being fully ported. The largest remaining work is porting the binary cache code, the `neighborhood` processing, and the GoLang account reconciliations.

## Specification

There were no changes to the [Specification for the Unchained Index](https://trueblocks.io/papers/2023/specification-for-the-unchained-index-v0.51.0-beta.pdf) since the last release.

## Breaking Changes

- Bumped version to v0.60.0.
- Bumped required GoLang version to 1.19.
- Removed the global `--to_file` option from all tools. You may use the `--output` option instead. Note that `--output` does not work when interacting through the API.
- Removed the rarely-used, global `--very_raw` option from all tools.
- Removed the global `--dollars` option from tools that still presented it. Its implementation was unsupportable. To reproduce the behaviour, use the `spotPrice` exported by reconciliations.
- During articulation, the export previously included certain whitespace characters direclty (`\t`, `\r` and `\n`, for example). This was causing various problems (for example, trying to import into CSV into MS Excel). The code now replaces those characters with `[t]` for `\t` or `[n]` for `\n`, changes `\r` into a space and (in some cases) changes `,` internal to a field into `_`. This may be a breaking change depending on your usage.
- Previously, if one exported traces to CSV or TXT, and smart contracts were both created and self destructed in a single trace (which happens frequently), the code used to report the newly-minted smart contract in the `from` field of the trace. This was not canoncial. We stopped doing it. If you wish to see the address of the newly-created contract in such a case, export to `--fmt json`. The value is presented as received from the RPC.

## Bug Fixes

- There were a number of fairly minor bug fixes including one to PetNames and the above mentioned correction to the index due to Erigon's bug.

## System Wide Changes

- We completed a full port to GoLang for `chifra names`, `chifra abis`, and `chifra receipts`. In some cases, this changed the format of the output (especially for JSON output). In every case, we think the data has been improved.
- We completed partial ports for `chifra blocks`, `chifra transactions`, and `chifra traces`. In some cases, this changed the format of the output (especially for JSON output). In every case, we think the data has been improved.
- Implemented `--articulate` across many tools in GoLang. (Thanks Dawid!)
- We made significant improvements to the documentation including more examples for the API docs and cross links to data models from tools producing the same.
- We prepared all tools for using the GoLang `--cache` options (caching is not yet yet enabled in the GoLang code). (Thanks Dawid!)
- Better support for streaming output to various formats (including preliminary support for `.xlsx`).
- Begun improvements for more useful and flexible connections to the RPC.

## Changes to Data Models

- `Function` data model:
  - Changed `abi_source` to `abiSource`.
  - Changed `input_dicts` to `inputDicts`.
  - Changed `output_dicts` to `outputDicts`.
  - Removed `input_names` and `output_names`. (These may be added back in in the future.)
- `Reconciliation` data model:
  - Changed `prevBlock` to `prevAppBlk`.
  - Changed `prevBlkBal` to `prevBal`.
  - Changed `getCostOut` to `gasOut`. (This may be renamed in the future.)
  - Added `accountedFor` field.
- `Block` data model:
  - Removed `unclesCnt`.
- `TraceResult` data model:
  - Renamed `newContract` to `address` in order to agree with the RPC.
- Renamed `VerboseAppearance` data model to `SimpleAppearance` to be consistent with other tools.
- Renamed `TokenBalanceRecord` data model to `TokenBalance`.
- New data models:
  - `BlockCount`
  - `ChunkAddresses`
  - `ChunkAppearances`
  - `ChunkBlooms`
  - `ChunkIndex`
  - `ChunkStats`
  - `NamedBlock`
  - `TraceCount`

## Tool Specific Changes

**chifra list**

- Added `--no_zero` option analogous to the same option for `chifra state`.
- Added `--first_record` and `--first_block` options analogous to the same option for `chifra export`.
- Added `--max_record` and `--last_block` options analogous to the same option for `chifra export`.

**chifra export**

- Major re-write of accounting module. Previously, token accounting was incomplete. Now, were' 99.98% accurate.
- Removed `--dollars` option. Instead, use the `spotPrice` from `chifra export --accounting` reconciliation model. 
- Clarified the semantics of `--first_record`, `--max_records`, `--first_block`, `--last_block` and how they interact.

**chifra monitors**

- Added `chifra monitors --list` option to display a list of currently monitored addresses.
- Added `chifra monitors --decache` to remove all files (including those in the cache) for a given address. Note that this may remove cache items that are associated with other monitors, as cache items (for example transactions and traces) are shared between addresses.

**chifra names**

- Completed port of the entire tool to GoLang
- Deprecated the `--named` option. Use the `--all` option instead. `--named` will be removed in a future release.
- Added new names to names database including OFAC censored names and addresses.
- Made preliminary steps in preparation for a much better names module.

**chifra abis**

- Removed the `--sol` option. Tools such as `foundry's forge` are much better. Use those.
- Added the `--clean` option to remove empty or unused abi files from the cache. Note that removing empty ABI files is not yet implemented.
- Added the `--encode` option to generate a 32-byte encoding for the given canonical function or event signature.

**chifra blocks**

- Changed the name of the `--trace` option to `--traces`. Deprecated `--trace` which will be removed in a future release.
- Improvements to `chifra blocks --count` to show more information about various data types.

**chifra transactions**

- Changed the name of the `--trace` option to `--traces`. Deprecated `--trace` which will be removed in a future release.
- Renamed the `--statements` option to `--account_for`. `--statements` no longer works.
- Added placeholder option called `--source` which meant to trace the source of funds into an address. Currenly not implemented.

**chifra receipts**

- Completed full port to GoLang.

**chifra logs**

- No notable changes.

**chifra traces**

- Removed the previous un-implemented `--statediff` option.
- Changed hot key from `-c` to `-U` for the `--count` option to be consistent with other tools.

**chifra when**

- Changed the `--repair` option to use the block range(s) provided on the command line for processing instead of a `<blockId>` directly.
- Changed hot key from `-c` to `-U` for the `--count` option to be consistent with other tools.

**chifra state**

- No notable changes.

**chifra tokens**

- Changed data model previously called `TokenBalanceRecord` to `TokenBalance`.

**chifra config**

- Renamed the `status` tool to `config` as its responsibilities have grown. `status` is now an alias of `config` and will continue to work as before.
- Previously named `modes` positional option now takes different values (`show` or `edit`). (`edit` is unimplemented.)
- Previous option for `modes` (`[index | monitors | names | abis | caches | some | all]`) are now available under the `--module` option for either `show` or `edit`.
- Removed the `--report` option which had been previously deprecated.
- Added `--paths` option which is useful for migration instructions.
- Minor improvements to reporting from this tool.

**chifra daemon**

- Renamed the `serve` tool to `daemon` as its responsibilities have grown. `serve` still works (and will continue to work) but only starts the API server. In the future, `daemon` will allow you to start other long running processes (i.e. daemons) such as the `chifra scrape` and `chifra monitor --watch` tools.
- Added option `--api` which takes on values of `[ on | off ]`.
- Added option `--scrape` which takes on values of `[ off | blooms | full-index ]`. Currently non-functional.
- Added option `--monitor` which eneables the monitor tool. Currently non-functional.

**chifra scrape**

- No notable changes.

**chifra chunks**

- Added `--save_addrs` option (currently hidden) to write addresses to disc from chunks to aide in debugging.
- Improved reporting for various options.

**chifra init**

- No notable changes.

**chifra explore**

- No notable changes.

**chifra slurp**

- Turned off testing for this tool as Etherscan availability was a very frequent cause of failed test cases. In order to test `chifra slurp` one must set an environment variable.

**makeClass**

- Removed `--tsx` option as unused.
- Removed `--dump` option as unused.
- Added `--sdk` option to output Python and Typescript SDKs.
- Separation of `CParameter` class from `CMember` class making publically presented `CParamater` much simple since most of the complications came from that class's use in makeClass.

**testRunner**

- Improved test runner to protect against incorrect test cases.
- Additions, corrections, removal of incorrect, and general improvements to test cases.

## v0.55.0

With this release, we made a lot of improvements to the help file and the code. Three tools have been completely or partially ported to GoLang. In general, the changes in this release are not breaking, however, as we port to GoLang various inconsistencies appear which we have (and will in the future) fix. We've tried to detail things as much as possible below. There are no migrations needed for this release.

## Specifications

- Updated the Specification for the Unchained Index to version 0.55.0. (Note this does not update the actual index chunks as this update does not change any algorithms or data structures used to create the index.)

## Breaking Changes
- The `--to_file` option has been removed from all tools.
- Value of the `--callType` field that previously held `suicide` now contain `self-destruct` throughout all tools.
- The `hash` field in the `Receipt` data model has been changed to `transactionHash`.
- The `logsBloom` field in various data models is no longer produced by `--raw` option.
- As some of our tools are only partially ported to Go (such as `chifra blocks` and `chifra traces`), some outputs differ in subtle ways depending on the options chosen.

## Bug Fixes
- Fixed a bug in `chifra export --neighbors` related to the display of that information.
- Fixed a bug related to `chifra scrape` that was not allowing forward progress of the scrape in certain situations.
- Various other small bug fixes.

## System Wide Changes

- Updated version to v0.55.0-alpha.
- Significant improvement to the help website.
- First introduction of very rudimentary TypeScript and Python SDKs (currenly pre-alpha and only supported for internal use).
- Removed a number of outdated or unused README files, other documentation, and source code.
- Removes rarely used and very confusing `--to_file` option from all tools. Accomplish the same thing with `--output <fn>`.
- Updated the names, timestamps, and manifest databases and published the same to Unchained Index.
- Removes a lot of very old addresses from the names database (that is non-core addresses that have not appeared on the chain since the arbitrarily chosen 2022-01-01T00:00:00.)

## Tool Specific Changes

**chifra blocks**
- The `--trace` option was renamed to `--traces`. `--trace` is deprecated and may be removed in the future.
- Expanded the `--count` option to produce additional counts for uncles and traces, etc.
- Partial port to GoLang. See note above.

**chifra traces**

- An attempt was made to improve the data exported from this tool, as it was quite confused previously. There may be unforeseen breaking changes to the expotred data.
- Removed unused (and previously unimplemented) `--statediff` option.
- Partial port to GoLang. See note above.

**chifra transactions**

- The `--trace` option was renamed to `--traces`. `--trace` is deprecated and may be removed in the future.

**chifra status**

- Renamed `chifra status` to `chifra config` to make the tool's purpose more clear. `chifra status` is deprecated and may be removed in the future.
- Added additional support for the docker version of the tools.
- Made `chifra status --terse` the default output.
- Added `--path` option to aid with the installation process.

**chifra serve**

- Renamed `chifra serve` to `chifra daemon` to make the tool's purpose more clear. `chifra serve` is deprecated and may be removed in the future.

**chifra chunks**

- Added `--save_addrs` option to export address lists from index database.
- Improved many features here to enable better exporting of the data in the Unchained Index.

## Data Model Changes

In an effort to produce better, more consistent data from all of our tools (and because we are not yet at version v1.0.0), we choose to modify the following fields and/or data models in a effort to make the data more consistent. Many of these changes are breaking.

**renamed data models**

- Renamed the `datedBlock` data model to `namedBlock` to more closely reflect its meaning.
- Renamed the `tokenBalanceRecord` data model to `tokenBalance` to be more terse and more clear.
- Renamed the `appearanceDisplay` data model to `appearance` removing confusing code. Fields within the new data model were changed or added to be more descriptive. (See below.)

**new data models**

- Added the `indexCacheItem` data model, which carries information about a single Unchained Index chunk as produced by `chifra scrape` and `chifra chunks`.
- Added the `blockCount` data model, which carries statistical information about blocks as produced by `chifra blocks --count`.
- Added the `traceCount` data model, which reports the number of traces in a transaction as produced by `chifra traces --count`.

**renamed fields**

- Renamed the `hash` field in the `Receipt` data model to `transactionHash`.

**new (or added) fields**

- Added `blockNumber`, `transactionIndex`, `transactionHash`, `blockHash`, and `isError` to `Receipt` data model.
- Added `traceIndex` and `reason` to the `Appearance` data model.
- Added `name`, `timeStamp`, and `date` fields to the `NamedBlock` data model.

**removed fields**

 - Removed the `root` field from the `Receipts` data model.

## Other

- Added four-byte cross product tool to produce a massive number of four-byte encodings (pre-alpha).
- Enabled auto-code generation for GoLang types, Python and Typescript SDKs, and other data models and tools.
- Improved and/or added test cases for most tools and apps.

## v0.45.0

Changes in this release are in support of [the docker version](https://github.com/TrueBlocks/trueblocks-docker). The changes are focused on two tools (`serve` and `status`) which have been renamed to `daemon` and `config`, respectively. This change allows these tools to more clearly reflect what they do.

### Bug Fixes

- Fix to `chifra export --accounting` reporting incorrect reconciliations.

### System-Wide Changes

- Updates version to v0.45.0-beta
- Near total rewrite of README for clarity and ease of use
- Added many tests for various tools

### Changes to Specific Tools

**chifra serve**

- **Breaking change**: `chifra serve` renamed to `chifra daemon`. All other options remain the same.

**chifra config (formerly chifra status)**

- Renamed `chifra status` to `chifra config`. `Chifra status` now reports that it's been deprecated and instructs the user to use `chifra config`.
- Prepares to use this tool for interactive editing of configuration files such as `git` and `ipfs` do.
- **Breaking change**: Moved existing positional options under `--module` flag which. Must be one of `[ index | monitors | names | abis | caches | some | all ]`.
- **Breaking change**: Added new positional options which must be on of `[ show | edit ]`.

**chifra export**

- Rewrote the way `--first_block`, `--last_block`, `--first_record`, and `--max_record` behave, especially how they interoperate.

**chifra scrape**

- Internal-only changes in preparation for starting, pausing, and restarting the scraper from `chifra daemon`. In all other ways, it operates identical to previous versions.

**chifra init**

- Internal-only changes in preparation for starting, pausing, and restarting the scraper from `chifra daemon`. In all other ways, it operates identical to previous versions.

**chifra monitors**

- Internal-only changes in preparation for starting, pausing, and restarting the scraper from `chifra daemon`. In all other ways, it operates identical to previous versions.

**chifra list**

- Added the `no_zero` option to instruct `chifra list` to not report any addresses for which there are no transactions (incomplete).

**chifra names**

- Added a names to the names database

**chifra blocks**

- Started process of porting `chifra blocks` to GoLang.

**chifra transactions**

- Rudimentary implementation of `--source` option.

**chifra version**

- Added `chifra version` which is identical to many other command line tools. Identical behaviour to `chifra --version` which still works.

## v0.44.0

### Significant Bug Fixes

- Fixes a significant bug related to scraping at the head of the chain that was introduced in v0.41.0. [Migration needed](./MIGRATIONS.md).

### System-Wide Changes

- Updates version to v0.44.0
- Implements first version of GoLang native binary cache (for `chifra receipts` only, but more soon).

### Changes to Specific Tools

- **chifra export:**
  - *Breaking change:* Changes default for `--first_block` to `1`. May have semantic effects.
  - Significant improvements to `--first_block/--last_block` and `--first_record/--max_record` and how they interact. Much better testing.

- **chifra list:**
  - Adds `--first_block/--last_block` and `--first_record/--max_record` to `chifra list`.

- **chifra scrape:**
  - Fixes a significant bug related to scraping at the head of the chain that was introduced in v0.41.0.
  - Re-enables `--file` option to fix broken docker build.

- **chifra serve:**
  - Disallows `--chain` option on command line. Not needed, as the API obeys the configuration file `defaultChain` setting, and the API serves any chain with the `--chain` option.

- **chifra traces:**
  - Changes `-c` shortcut to `-U` to be more consistent with other tools.

- **chifra when:**
  - Changes `-u` shortcut to `-U` to be more consistent with other tools.
  - Changes previously hidden `--repair` option to a switch and picks up block identifiers from the command line.

## v0.43.0

### System-Wide Changes

- Bump version to v0.43.0
- Better support for `--file`, `--to_file` (since removed), and `--output` options for all tools.
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
- Better support for `--file`, `--to_file` (since removed), and `--output` options for all tools.
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
