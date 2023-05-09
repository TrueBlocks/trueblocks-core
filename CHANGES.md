<!-- markdownlint-disable MD024 MD036 -->
# History of Changes

This file details changes made to TrueBlocks per version. See the [migration notes](./MIGRATIONS.md) for changes from previous versions.

## v0.65.0 (2023/05/10)

Added optional gRPC server for `chifra names` route.

## v0.64.0 (2023/04/20)

This is not a minor release that closes a few random issues and adds a small feature to better manage the caches.

## Issues closed

- Bumps version to v0.64.0-beta
- Fixes an undocumented issue with publishing the website documentation.
- Closes 8 or 9 other minor issues.

## Changes to Data Model

- Adds `date` and exposed `timestamp` fields on `Logs` data model.

## Changes to Tools

- Adds `--decache` option to `chifra blocks` and `chifra transactions` tools.

## v0.62.0 (2023/04/12)

This release focuses mostly on porting the C++ code to GoLang. We're almost done. The largest remaining work is porting the binary cache code, the `neighborhood` processing, GoLang account reconciliations, `chifra state`, `chifra tokens`, and `chifra export`. We hope to complete this work in the next month or two. Then--on to optimization!

## Specification

There were no changes to the [Specification for the Unchained Index](https://trueblocks.io/papers/2023/specification-for-the-unchained-index-v0.51.0-beta.pdf) since the last release.

## Breaking Changes

- Bumped version to v0.62.0.
- Removes support for all migrations and versions prior to v0.40.0.
- There were breaking changes in the `chifra blocks`, `chifra transactions`, `chifra logs`, `chifra traces`, `chifra state`, `chifra config`, `chifra status`, and `chifra chunks` tools. See notes below.

## System Wide Changes

- Ported to GoLang: `names`, `slurp`, `logs`, `traces`, `daemon`, `config`, `status`
- Removes classDefinition code from c++ library testing code - not used not needed
- Many additional tests for all subcommands
- Removed a fair amount of the C++ library testing code as being not needed and in preparation for porting to C++
- Re-wrote logger package to more closely mimic the new GoLang structured log package which we will be switching to soon - if you depend on our logging messages for anything, please note that they will change.
- Made sure RPC and Raw data agrees

## Changes to Data Models

- `Blocks`:
  - Added `uncles` field.
  - added `author`, `receiptsRoot`, `sha3Uncles`, `size`, `stateRoot`, `totalDifficulty`, `extraData`, `logsBloom`, `mixHash`, `nonce`, `transactionsRoot`, `uncles` to `chifra blocks --raw`.
  - removed finalized -- the user can make this decision themselves based on `timestamp` or `blockNumber`.
- `ChunkBloom`:
  - Renames `count` to `nBlooms` to be consistent with other tools.
  - Renames `width` to `byteWidth` to be consistent with other tools.
- `ChunkIndex`:
  - Renames `addressCount` to `nAddresses` to be consistent with other tools.
  - Renames `appearanceCount` to `nAppearances` to be consistent with other tools.
- `ChunkRecord` (formerly `PinnedChunk`:
  - Removes `firstApp` and `lastApp`. Same data is available from the `range` field.
  - Added `bloomSize` and `indexSize` which is useful for cache management.
- `Function`
  - Made `stateMutability` and `signature` be omitempty
- `Log`
  - Made `topics`, `data`, `articulatedLog`, `compressedLog`, and `timestamp` omitempty
  - Removed `transactionLogIndex` field as unused.
- `Receipt`
  - Adds `logsBloom` but only for `chifra receipts --raw`
- `Trace`
  - Makes `articulatedTrace` field omitempty
- `Transaction`
  - Added `accessList` and `chainId` to `chifra transactions --raw`
- The data models related to `chifra status` and `chifra chunks` have been modified in various ways.
- Renamed data models
  - `Cache` to `Status`
  - `CacheEntry` to `CacheItem`
  - `ChunkAddresses` to `ChunkAddress`
  - `ChunkAppearances` to `ChunkAppearance`
  - `ChunkBlooms` to `ChunkBloom`
  - `PinnedChunk` to `ChunkRecord`
- New data models:
  - `Bounds` (for `chifra list --bounds`)
  - `Etherscan` (for `chifra slurp`)
  - `MonitorClean` (for `chifra monitors --clean`)
  - `ReportCheck` (for `chifra chunks index --check`)
  - `Timestamp` (for `chifra when --timestamps`)
  - `TimestampCount` (for `chifra when --timestamps --count`)
  - `TraceFilter` (for `chifra traces <filter>`)

## Tool Specific Changes

**chifra list**

- Adds `--bounds` option to show first and latest appearance, timestamps and dates.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Improves date output format to agree with other tools.
- Removes quotes from csv files (same for all other tools that are ported to GoLang).

**chifra export**

- No longer errors out if the node software fails to deliver valid data. This allows for continuing to stream data even when the node is flaky (Erigon). This is in preparation for better error handling.

**chifra monitors**

- No appreciable changes.

**chifra names**

- Ported entirely to GoLang.
- Removes `--to_custom` option as unused.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Much better support for CRUD operations.
- Preparation for switching to a gRPC server for names for both API and command line versions.
- `--autoname` and `--clean` options are temporarily suspended. Will be re-enabled later.

**chifra abis**

- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.

**chifra blocks**

- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Now displays all fields when using `--raw` option.
- **Breaking:** Removes support for previously deprecated `chifra blocks --trace` option. Use `chifra blocks --traces` instead.
- **Breaking:** Removes `finalized` field as it was incorrect and unused

**chifra transactions**

- Better JSON exporting, better formatting, now obeys `omitempty` for `maxFeePerGas`, `maxPriorityFeePerGas`, `nonce`, `articulatedTx::inputs`, and `articulatedTx::outputs` fields.
- Now displays all fields (including `accessList`) when using `--raw` option.
- **Breaking:** Removes support for previously deprecated `chifra transactions --trace` option. Use `chifra transactions --traces` instead.
- **Breaking:** No longer displays `r`, `s`, or `v` values in `--raw` display.

**chifra receipts**

- Ported entirely to GoLang.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Adds `blockHash` and `transactionHash` to JSON output.
- Adds `logsBloom` to `--raw` output.

**chifra logs**

- Ported entirely to GoLang.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Added `timestamp` to the export.
- **Breaking:** Removes RPC results wrapper from `--raw` output.

**chifra traces**

- Ported entirely to GoLang.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields. Does not export field if it holds its default value (except for arrays which export `[]`).
- **Breaking:** Removed previously unused `--skip_ddos` and `--max` options permanently.
- **Breaking:** Fixed incorrect JSON output for the `traceAddress` field - previously we export a string. We now export an integer array.
- **Breaking:** The `value` field is now exported as a string to preserve accuracy. Previously exported numbers which JavaScript could not properly parse.
- **Breaking:** A previously generated "fake" trace with `callType == "creation"` was removed. This trace was manufactured by TrueBlocks to indicate a smart contract creation. Now, instead, we export the actual trace returned by the RPC which contains a `callType` of `"creation"` but also includes the `init` code data in the `traceAction` and the `code` in the `traceResult`.

**chifra when**

- Better documents `--timestamp` related options.

**chifra state**

- **Breaking:** Removes `nonce` from the `parts` option (may return at a future date).

**chifra tokens**

- Removes support for previously deprecated `none` option to `chifra tokens --parts`. No replacement.

**chifra config**

- **Breaking:** This tool has been temporarily suspended. It will re-emerge soon as a way to edit and display configuration information.
- All features previously available with this tool are now available under the previously aliased `chifra status` tool.

**chifra status**

- Ported entirely to GoLang.
- Separated `status` tool from `config` tool. `status` tool now displays information about the caches. `config` is now related to configuration information, but is currently disabled.
- Many breaking changes to the data types and data formats produced by this `admin` specific tool. Instead of documenting here, please see the documentation for details.
- Expanded types of cache reports produced to now include `[ index | blooms | blocks | txs | traces | monitors | names | abis | recons | slurps | staging | unripe | maps | some | all ]`
- Adds `--first_record` and `--max_records` to limit reporting of large caches.
- **Breaking:** Removes the `--parts` option due to expansion of `modes` option.

**chifra daemon**

- Changed `chifra daemon --scrape full-index` to `chifra daemon --scrape index`.
- Began process of enabling `chifra daemon --monitors` and `chifra daemon --scrape`. Not yet ready for prime time.

**chifra scrape**

- No appreciable changes.

**chifra chunks**

- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Possible breaking changes to field names of some exported data. Please see the documentation for this tool if you experience problems.
- **Breaking:** Export `range` field as a string not an object.
- **Breaking:** Removes `--save_addrs` option as unused.
- **Breaking:** Removes `status` option from `chifra chunks` command as unused - use `chifra status --verbose` instead.
- **Breaking:** `chifra chunks --belongs` now only works with `--fmt json`.

**chifra init**

- No appreciable changes.

**chifra explore**

- No appreciable changes.

**chifra slurp**

- Ported entirely to GoLang.
- Better JSON exporting, better formatting, now obeys `omitempty` for all appropriate fields.
- Added `--per_page` to control how many records to ask for from Etherscan.
- Added `--sleep` to slow down access to Etherscan.
- Added `1155` option to the `types` option.
- Added a number of test cases to better cover the code.

**makeClass**

- No appreciable changes.

**testRunner**

- No appreciable changes.

## Issues Closed (27)

- [#2755 - fatal error: concurrent map writes caused by accessing chifra daemon api server via http](https://github.com/TrueBlocks/trueblocks-core/issues/2755)
- [#2813 - chifra list if control+c is hit leaves an artifact in the `monitors/staging` folder and then adds dups if re-started.](https://github.com/TrueBlocks/trueblocks-core/issues/2813)
- [#2817 - chifra list with a very large address (such as uniswap) would leave the temp monitor file in staging when Control+C](https://github.com/TrueBlocks/trueblocks-core/issues/2817)
- [#2494 - Articulation Epic](https://github.com/TrueBlocks/trueblocks-core/issues/2494)
- [#2439 - --file Epic](https://github.com/TrueBlocks/trueblocks-core/issues/2439)
- [#2441 - chifra cmd Does --output <fn> get closed after each round in the case of --file?](https://github.com/TrueBlocks/trueblocks-core/issues/2441)
- [#2443 - chifra cmd some --file tests remain turned off](https://github.com/TrueBlocks/trueblocks-core/issues/2443)
- [#2784 - chifra logs - port to GoLang](https://github.com/TrueBlocks/trueblocks-core/issues/2784)
- [#2284 - chifra status in Api mode is missing some data for status](https://github.com/TrueBlocks/trueblocks-core/issues/2284)
- [#2305 - chifra status --migrate index should be removed since it doesn't do anything](https://github.com/TrueBlocks/trueblocks-core/issues/2305)
- [#2734 - chifra config - undeprecate status](https://github.com/TrueBlocks/trueblocks-core/issues/2734)
- [#2416 - chifra status --terse improvements](https://github.com/TrueBlocks/trueblocks-core/issues/2416)
- [#2307 - chifra status --terse should be the default](https://github.com/TrueBlocks/trueblocks-core/issues/2307)
- [#2831 - remove custom name integration test file](https://github.com/TrueBlocks/trueblocks-core/issues/2831)
- [#2823 - chifra modeler speedup thoughts](https://github.com/TrueBlocks/trueblocks-core/issues/2823)
- [#2822 - chifra all - speed thoughts](https://github.com/TrueBlocks/trueblocks-core/issues/2822)
- [#2821 - chifra all - speed thoughts](https://github.com/TrueBlocks/trueblocks-core/issues/2821)
- [#2797 - chifra articulate - make maxFeePerGas and maxPriorityFeePerGas omitempty](https://github.com/TrueBlocks/trueblocks-core/issues/2797)
- [#1790 - chifra cmd things we can parallelize](https://github.com/TrueBlocks/trueblocks-core/issues/1790)
- [#2787 - chifra logs - finish GoLang port](https://github.com/TrueBlocks/trueblocks-core/issues/2787)
- [#2238 - chifra cmd we need to do a much better job logging](https://github.com/TrueBlocks/trueblocks-core/issues/2238)
- [#2360 - chifra cmd lots of nice ideas here](https://github.com/TrueBlocks/trueblocks-core/issues/2360)
- [#2585 - chifra cmd - When porting to Go is done, store addresses and hashes as binary](https://github.com/TrueBlocks/trueblocks-core/issues/2585)
- [#2663 - chifra other Question on Beacon Chain Issuance over time range for @ethburnbot integration](https://github.com/TrueBlocks/trueblocks-core/issues/2663)
- [#2672 - chifra cmd --decache option should be available for more thing](https://github.com/TrueBlocks/trueblocks-core/issues/2672)
- [#2650 - chifra monitors does --decache option remove abi files as well. It should.](https://github.com/TrueBlocks/trueblocks-core/issues/2650)
- [#2677 - chifra cmd Timestamp is confusingly uint64 and int64 and is cast about](https://github.com/TrueBlocks/trueblocks-core/issues/2677)

## Issues Opened (35)

- [#2851 - chifra init - protect against control+c while download timestamp file](https://github.com/TrueBlocks/trueblocks-core/issues/2851)
- [#2850 - chifra list --bounds seems unnecessarily slow](https://github.com/TrueBlocks/trueblocks-core/issues/2850)
- [#2849 - chifra monitors --watch](https://github.com/TrueBlocks/trueblocks-core/issues/2849)
- [#2848 - chifra status -- saving old behaviour pre-port to golang](https://github.com/TrueBlocks/trueblocks-core/issues/2848)
- [#2847 - chifra export creates huge number of abi files when scraping UniSwap contracts](https://github.com/TrueBlocks/trueblocks-core/issues/2847)
- [#2846 - chifra monitors -- add deleted status to the fields](https://github.com/TrueBlocks/trueblocks-core/issues/2846)
- [#2845 - chifra monitors -- add --count option](https://github.com/TrueBlocks/trueblocks-core/issues/2845)
- [#2844 - chifra config - add --chains option](https://github.com/TrueBlocks/trueblocks-core/issues/2844)
- [#2843 - chifra transactions (and others) should export `nonce` field.](https://github.com/TrueBlocks/trueblocks-core/issues/2843)
- [#2841 - how to integrate gRPC types](https://github.com/TrueBlocks/trueblocks-core/issues/2841)
- [#2839 - golang IsArchiveNode always returns true. should not](https://github.com/TrueBlocks/trueblocks-core/issues/2839)
- [#2837 - chifra names on rpc branch reports errors when running from command line](https://github.com/TrueBlocks/trueblocks-core/issues/2837)
- [#2836 - chifra export --pending](https://github.com/TrueBlocks/trueblocks-core/issues/2836)
- [#2835 - chifra config does not report is_scraping when run through API, but it does when run through command line](https://github.com/TrueBlocks/trueblocks-core/issues/2835)
- [#2834 - chifra list does not have an --unripe flag](https://github.com/TrueBlocks/trueblocks-core/issues/2834)
- [#2826 - chifra list add --latest and --first options to report on latest or first appearances](https://github.com/TrueBlocks/trueblocks-core/issues/2826)
- [#2820 - chifra all - speed thoughts](https://github.com/TrueBlocks/trueblocks-core/issues/2820)
- [#2819 - chifra list ignores --first_block / --last_block](https://github.com/TrueBlocks/trueblocks-core/issues/2819)
- [#2818 - chifra chunks index --check --fmt txt cores](https://github.com/TrueBlocks/trueblocks-core/issues/2818)
- [#2815 - uniswap info - multicall](https://github.com/TrueBlocks/trueblocks-core/issues/2815)
- [#2814 - chifra list (if interupted) does not clean up](https://github.com/TrueBlocks/trueblocks-core/issues/2814)
- [#2810 - chifra state should show date](https://github.com/TrueBlocks/trueblocks-core/issues/2810)
- [#2806 - chifra caches could be SQLite databases](https://github.com/TrueBlocks/trueblocks-core/issues/2806)
- [#2804 - chifra scrape should make IPFS url and others configurable](https://github.com/TrueBlocks/trueblocks-core/issues/2804)
- [#2800 - chifra slurp date format when exporting to csv is wrong](https://github.com/TrueBlocks/trueblocks-core/issues/2800)
- [#2799 - chifra slurp cache is turned off](https://github.com/TrueBlocks/trueblocks-core/issues/2799)
- [#2794 - chifra list address --verbose date format is inconsistent with other export](https://github.com/TrueBlocks/trueblocks-core/issues/2794)
- [#2793 - chifra traces conflicting names have added numeral when articulating](https://github.com/TrueBlocks/trueblocks-core/issues/2793)
- [#2791 - chifra trace change breaks reconciliation](https://github.com/TrueBlocks/trueblocks-core/issues/2791)
- [#2790 - chifra names Go port and long running process](https://github.com/TrueBlocks/trueblocks-core/issues/2790)
- [#2789 - chifra traces - implement --articulate in GoLang](https://github.com/TrueBlocks/trueblocks-core/issues/2789)
- [#2786 - chifra logs data model does not show blockNumber, transactionIndex, or transactionHash when rendered under a receipt](https://github.com/TrueBlocks/trueblocks-core/issues/2786)
- [#2785 - chifra logs - logs with messages in them](https://github.com/TrueBlocks/trueblocks-core/issues/2785)
- [#2782 - chifra build - binary releases](https://github.com/TrueBlocks/trueblocks-core/issues/2782)
- [#2777 - chifra test - remove slow tests](https://github.com/TrueBlocks/trueblocks-core/issues/2777)

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
- Added option `--scrape` which takes on values of `[ off | blooms | index ]`. Currently non-functional.
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
