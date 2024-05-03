# History of Changes

This file details changes made to TrueBlocks over time. See the [migration notes](./MIGRATIONS.md) for any required actions you must take to stay up to date.

## v2.5.9 (2024/04/05)

- Bumped version to 2.5.9.
- Separates out tests into its own submodule.

## v2.5.8 (2024/02/09)

- Bumped version number to v2.5.8.
- Bumped required GoLang version to ^1.22.

**bug fixes**

- Fixes very slow `chifra blocks --traces` command. Up to 10x faster.
- Complete re-write `--accounting` reconciliation code. Still a work in progress, but improved.

**chifra export**

- Significant speedup due to fully using goLang concurrency better. Up to 10x faster in some cases.

**chifra scrape**

- Added the notify feature (currently undocumented) to help enable TrueBlocks Key.

**chifra abis**

- Added `--proxyFor` option to allow for redirecting query for ABI files to the implementation.

**chifra transactions**

- Removed previously unused `accountFor` option.
- Changed previously (and still) unused `--source` option to `--seed`. `--seed` option traces the source of funds.

**chifra slurp**

- Added `--articulate` option to attempt to articulate transactions.
- Added `--count` option to return the count of transactions found for an address.
- Added `--source` option to allow for both Etherscan and Key as sourcef for data.

**chifra logs**

- Added `--emitter` and `--topic` options to allow for filtering logs by emitter and topic. Now agrees with `chifra export`.

data models

- Added `ReconType` and `AssetType` to `Reconciliation` data models.

## v2.1.0 (2023/11/25)

**Summary**

No migrations required.

## Changes to the Specification

- None.

## Breaking Changes

- None.

## System Wide Changes

- Improvements to validation code for many commands. None breaking.
- Fixed a potential bug when articulating token approvals and ENS token transfers in various tools. (Potentially breaking, but corrects a previous bug.)
- Tries to remove extraneous downloads of timestamps database, plus allow for Control+C to cancel the download.

## Changes to Data Models

The following data models were either modified, added, removed, or renamed by having their fields added to, removed, or renamed. Please consult the documentation on our website for details.

### Modified data models

| model         | description                                                                                                     |
| ------------- | --------------------------------------------------------------------------------------------------------------- |
| `ChunkRecord` | Automates previously hand-edited model                                                                          |
| `Status`      | Automates previously hand-edited model                                                                          |
| `Trace`       | Docs now report `traceAddress` as an integer array not a string (documentation change only)                     |
| `TraceFilter` | Automates previously hand-edited model. Corrects this previously undocumented model.                            |
| `Config`      | Automates previously hand-edited model. Many corrections in support of the `chifra stastus --diagnose` feature. |

- **Note:** The `Config` data model will be removed in a future release. Most of its function is in `Status`.

### New data models

| model | description |
| ----- | ----------- |
| None  |             |

### Removed data models

| model      | description                                        |
| ---------- | -------------------------------------------------- |
| `Abi`      | Removes a documented but never produced data model |
| `Key`      | Removes a documented but never produced data model |
| `Transfer` | Removes a documented but never produced data model |

### Renamed data models

| model | description |
| ----- | ----------- |
| None  |             |

## Tool Specific Changes

**chifra list**

- No changes.

**chifra export**

- Adds `--reverted` option to `chifra export` to allow for exporting only reverted transactions.
- Fixed a bug wherein `chifra export --traces --cache` was not caching the traces.
- Slightly better control of filtering (including `--reverted` option) with `chifra export.` More coming soon...
- First attempt to avoid downloading huge number of ABI files for `factory` contracts. (Didn't work, but improved code base.)

**chifra monitors**

- Various small improvements to `chifra monitors --watch` including better error handling of output formatting and progress reporting.

**chifra names**

- No changes.

**chifra abis**

- Better handling of downloaded ABI files including versioning.

**chifra blocks**

- No changes.

**chifra transactions**

- No changes.

**chifra receipts**

- No changes.

**chifra logs**

- No changes.

**chifra traces**

- No changes.

**chifra when**

- No changes.

**chifra state**

- Improvements to calling conventions for `chifra state --call` including allowing ENS names in function call parameters.
- Fixes an issue when querying balances against some nodes due to return of hex when an integer was expected.

**chifra tokens**

- Fixes an issue when querying token balances against some nodes due to return of hex when an integer was expected.

**chifra config**

- No changes.

**chifra status**

- Adds `--diagnose` option to `chifra status` to allow for more detailed reporting of the status of the system.

**chifra daemon**

- Adds `--url` option to `chifra daemon` to allow for specifying the URL of the node to connect to. (Replaces `--port` option.)
- Deprecates `--port` option in `chifra daemon`. Use `--url` option instead.

**chifra scrape**

- Added `notify` feature to `chifra scrape` to allow for notifying remote servers (for example, AWS) of new appearances.
- Added new `notifyUrl` configuration item to `[settings]` group in the config file.

**chifra chunks**

- Better control of user hitting Control+C during `chifra chunks --tag` operation. Not breaking.

**chifra init**

- No changes.

**chifra explore**

- No changes.

**chifra slurp**

- No changes.

**makeClass**

- Greatly simplifies (by removing unused features) auto code generation in preparation for port to GoLang.

**testRunner**

- No changes.

## Pull Requests (19)

<!--
gh pr list --search "is:pr is:closed closed:>2023-11-25" --limit 300 --state merged | cut -f1,2 | sed 's/^/- #/' | tr '\t' ' '
-->

- #3419 Bumps version to v2.1.0-release
- #3417 Improves auto-code gen
- #3416 chifra status --diagnose
- #3413 Adds --reverted to chifra export
- #3412 Fixes issue #2675
- #3407 Cleaning
- #3404 Appearance notification
- #3403 Fix/issue 2847
- #3402 Fix/other issues
- #3401 Closes the issue
- #3399 Allows hitting control+c when downloading timestamps
- #3397 Fixes issue #3376
- #3396 Bugfix/call tests
- #3395 3393 why an etherscan key
- #3391 Improve state call 3
- #3390 Adds a few tests for allowing ens names where previously only hex add…
- #3389 Improve state call 2
- #3388 Improve state call
- #3387 Improve monitoring

## Issues Closed (58)

<!--
gh issue list --search "closed:>2023-11-25 is:closed is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/' | tr '\t' ' '
-->

- #3409 unchained index - study the characteristics of the appearances
- #3408 chifra scrape - should allow for building an index that ignore traces
- #3400 chifra init missing index publisher
- #3394 chifra init - progress reporting
- #3393 chifra cmd - WHY DO WE NEED ETHERSCAN KEY ON BRAND NEW INSTALL?
- #3392 chifra config - use os.UserDir package code
- #3381 chifra init doesn't download timestamps file even though it's pushed
- #3377 chifra cmd - search for "Why do we need to do this here" for a question about loading the timestamps file
- #3376 chifra chunks index --tag should update the manifest with the version and the spec
- #3361 chifra export - fake withdrawal tx is incorrect
- #3359 chifra when - finalized
- #3347 chifra export - example miner txs that doesn't balance
- #3346 chifra cmd - all cachable items need a function NeedsUpgrade
- #3343 chifra scrape - should we be scraping access lists?
- #3338 chifra cmd - notes from call with Dawid
- #3325 chifra export -- should take dates as well as blocks
- #3310 chifra cmd - can't kill download of timestamps
- #3302 chifra transactions - Missing data in output
- #3282 chifra scrape - tracking issue for Notify
- #3267 chifra daemon --port issues
- #3266 chifra cmd - caching could possibly be a configurable option
- #3245 chifra cmd - the --publisher option should be global
- #3222 chifra export should handle withdrawals in accounting
- #3217 chifra cmd - every command wants to download timestamps
- #3209 chifra status - Improve UX of `chifra status` Progress row
- #3118 chifra export - decache should remove empty folders
- #3069 chifra blocks -- if not cached, this routine is very slow for late blocks
- #3021 chifra state - can we resolve ENS names in function call parameters?
- #2959 chifra cmd - we need test cases to protect against breaking changes in the RPC data
- #2934 chifra export - phony transfers
- #2927 chifra chunks --publish summary of issues
- #2920 chifra cmd - add http proxy setting for etherscan request.
- #2882 chifra cmd - first_record/max_records
- #2847 chifra export creates huge number of abi files when scraping UniSwap contracts
- #2841 How to integrate gRPC types
- #2806 chifra caches could be SQLite databases
- #2769 chifra other - don't forget about four-bytes and known_abi_gen
- #2675 chifra export --traces doesn't check the cache without --cache_traces
- #2664 chifra export --neighbours flag incomplete return when providing multiple addresses. (API & Chifra)
- #2575 chifra cmd - GoLang logging
- #2568 chifra cmd formatting
- #2562 chifra export Add filtering on output address for chifra export
- #2553 chifra export accounting mint/burn
- #2550 chifra export allow to filter failed transactions for an address
- #2546 chifra export discrepancies between docs and actuality in returned data
- #2525 chifra export does chifra export <address> --traces --cache --cache_traces actually work to create trace cache items
- #2523 chifra cmd - articulation: ABIs download and cache APIs
- #2453 chifra export customized export formats
- #2361 chifra cmd don't hide error messages from client
- #2333 chifra cmd update our code as per this
- #2322 chifra cmd control+c doesn't work well
- #2314 chifra cmd possible mode of operation
- #2306 chifra export control+c doesn't work
- #2299 chifra config - make this tool more capable
- #2295 chifra cmd Use of panic vs. fatal
- #2269 chifra cmd protect against overwriting files on disc
- #2237 chifra cmd reverse the polarity of the file backup mechanism
- #2187 chifra monitors --watch allow other commands than export

## Issues Opened (4)

<!--
gh issue list --search "created:>2023-11-25 is:open is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/'  | tr '\t' ' '
-->

- #3418 chifra export - unreconciled txs I
- #3415 notifyUrl should have global default value and per-chain config like defaultGateway
- #3414 chifra scrape should not die if the listener dies and notifyUrl is not empty
- #3411 chifra build - add nilaway to our CI

## v2.0.0 (2023/11/14)

**Withdrawals!**

This version includes many improvements over `version 1.0.0`, including support for reading and indexing CL withdrawal appearances. Because the index now more appearances, we were forced to make a breaking change to the index. Therefore, we bumped the major version number and a migration is required. There are many other new features and improvements listed below.

Complete [the migrations](./MIGRATIONS.md) now if you've not already.

The primary improvements with version 2.0.0 are:

1) Inclusion of previously missing `withdrawal` appearances, and
2) Much improved `chifra scrape` command which is now more inclusive, more performant, and more complete.
3) A much better `chifra monitors --watch` command which is now easier to use and easier to automate.
4) Greatly improved `chifra init` command with better reporting and progressive backoff to avoid rate limiting.
5) Improved local caching including fixes to certain performance bottlenecks.

Below is an explicit list of all changes.

## Changes to the Specification

We re-wrote the [Specification for the Unchained Index](https://trueblocks.io/papers/2023/specification-for-the-unchained-index-v2.0.0-release.pdf) to include all version 2.0.0 modifications. A reference to this version has been inserted into each chunk in the Unchained Index and included in the manifest (which is why there's the need for a migration).

## Breaking Changes

The following breaking changes were made for this release:

- Bumped version number to v2.0.0.
- Modifed the contents of the Unchained Index chunks in the following ways:
  - Updated the header of each chunk to reference `trueblocks-core@v2.0.0-release` (requires regeneration or re-downloading of index),
  - Includes previously missing `withdrawal` appearances enabled at the Shanghai hard-fork,
  - Removes `erigon` specific "hacks" required by previous (broken) version of `erigon`.
  - Includes previously missing appearearances for "misconfigured miners" in very early blocks (prior to block `100,000`).
- Near complete re-write of `chifra monitors --watch` option including breaking changes to command line functionality.
- Minor breaking changes to some data models as detailed below.
- Further removal or deprecation of esoteric, rarely used, and/or redundant options (see below).

## System Wide Changes

- Updated Specification of the Unchained Index v2.0.0.
- Freshened and re-published the Unchained Index `manifest` for `mainnet` and `sepolia` chains to the latest block.
- Greatly improved pinning interface including easier pinning of other chains and better management of existing pins.
- Exposed `--publisher` option to tools for which it makes sense as an aide in using alterative versions of the Unchained Index.
- Better error handling throughout including much better error handling from the RPC.
- Improved handling of user cancels (i.e., hitting `Cntl+C`) during caching and other processes to help avoid corrupting the databases.
- Much better debugging of internal curl calls including generation of `curl` command debugging replay files.
- Added function level enter/exit tracing for easier debugging.
- Improved formatting of outputs, expansion of the use of the `--verbose` option.
- Inclusion in most data output (where applicable) both timestamp and date fields (a user-suggested improvement).
- Removes `--wei` options from all commands as unnecassary. Breaking change. If used, simply remove it.
- Added documentation globally available options such as `--fmt`, `--ether`, `--cache`, `--no_header`, etc.
- Exposed a number of previously hidden options and commands (see below).

## Changes to Data Models

The following data models were either modified, added, removed, or renamed by having their fields added to, removed, or renamed. Please consult the documentation on our website for details. Some of these changes are breaking.

### Modified data models

| model        | description                                                                           | breaking |
| ------------ | ------------------------------------------------------------------------------------- | -------- |
| `Block`      | Added `withdrawals` array as per the Shanghai hard-fork.                              |          |
| `Manifest`   | Renamed `schema` field to `specification`.                                            | yes      |
| `Manifest`   | Remove previously unused `database` field.                                            | yes      |
| `Slurp`      | Added `withdrawalIndex` and `validatorIndex` to service the `--withdrawals` option.   |          |
| `Withdrawal` | Added `timestamp`, `date`, and `blockNumber` to `withdrawal` data model.              |          |
| `Chain`      | Removed `apiProvider` field from `Chain` data model as unused.                        | yes      |
| `Config`     | Internal changes to better maintain the `config` data model used in the `.toml` file. |          |

### New data models

| model     | description                                                                                        |
| --------- | -------------------------------------------------------------------------------------------------- |
| `IpfsPin` | New model with fields `cid`, `datePinned`, `status`, `size`, `fileName` for use in the `manifest`. |

### Removed data models

| model | description |
| ----- | ----------- |
| None  |             |

### Renamed data models

| model | description |
| ----- | ----------- |
| None  |             |

## Tool Specific Changes

**chifra list**

- Added `chifra list --decache` option to remove exising monitors and all associated cached items. Note that there is no corresponding `--cache` option for `chifra list`.
- Added `chifra init --publisher` option to allow for using alternatively published versions of the Unchained Index.

**chifra export**

- Added `chifra init --publisher` option to allow for using alternatively published versions of the Unchained Index.

**chifra monitors**

- Near complete re-write of the `chifra monitors --watch` tool (including breaking changes).
- Added `chifra monitors --watch --watch_list` to specify which addresses to watch. Previously the tool looked for a file in the local folder.Improves automation.
- Added `chifra monitors --watch --commands` option to carry the commands to run against the `watch_list`. Improves on existing poorly functioning and poorly documented commands file which used the `--file`.
- Added optional `--batch_size` and `--run_count` options to `chifra monitors --watch` to aide in debugging and performance testing.
- Renamed `--run_once` to `--run_count`. You may get the same behaviour with `chifra monitors --watch --run_count 1`.

**chifra names**

- Updated a (very) few names in the database.

**chifra abis**

- Added `chifra abis --cache` and `chifra abis --decache` options replacing previously available `--clean` option. `--cache` is a no-op as any downloaded ABI files have always been cached by default.
- Removed `chifra abis --clean` as inconsistent - use `chifra abis --decache` instead.

**chifra blocks**

- Fixed a major bottleneck in the RPC processing for this command to greatly speed up processing time.
- Added `chifra blocks --withdrawal` to display withdrawal transactions in a block.
- Added `chifra blocks --ether` to display balances and amounts in Ether (defaults to Wei otherwise).

**chifra transactions**

- Added `chifra transactions --ether` to display balances and amounts in Ether (defaults to Wei otherwise).

**chifra receipts**

- No changes.

**chifra logs**

- No changes.

**chifra traces**

- Added `chifra traces --ether` to display balances and amounts in Ether (defaults to Wei otherwise).

**chifra when**

- Improved `chira when --list` command to be more informative.
- Added a `chifra when --verbose` mode to be even more informative than that.
- Added additional `special` blocks and better descriptions for each block.
- Changed names of some rarely used `special` blocks (mildly breaking).

**chifra state**

- Improvements to the testing and performance of `chifra state --call` option.

**chifra tokens**

- No changes.

**chifra config**

- Near complete re-write of configuration modules and how they work
- Forces `v2.0.0-release` into the TrueBlocks.toml file (elicits an automatic migration of the toml file to current versions).
- Added `chifra config show` to dump the `trueBlocks.toml` config file to screen.
- Enables `chifra config edit` option to open an editor on the toml file (requires EDITOR environment variable to be set).
- Merges separate `blockScrape.toml` config files into the `per-chain sections` of main config file.

**chifra status**

- Better display of more information.

**chifra daemon**

- Fixed an issue with the server where `Content-Type` was being incorrectly forwarded in certain error conditions.

**chifra scrape**

- Near complete re-write of the scraper and the specification to fix various issues including indexing `withdrawals`.
- Much easier to understand (and document) unique address identifier, marking of mis-configured addresses, etc.
- Fixed an issue where scraper was missing certain smart contract addresses created during out of gas transactions in the early chain.
- Fixes many issues with scraper. It is now more complete, faster, and more consistent when running near the head of the chain.
- Remove `--pin` and `--remote` options from `chifra scrape`. Use `chifra chunks manifest --pin --remote` (post-de-facto) instead.
- Replace `chifra scrape --first_block` option with `chifra scrape --touch` 
- Renamed `--run_once` to `--run_count`. Get same behaviour with `chifra --run_count 1` - aides in debugging scraper.
- Added `--dry_run` to aide in debugging scraper.
- Now disallows running `chifra scrape` if the node is not a tracing archive node.
- Changed the name of the config item `allow_missing` to `allowMissing`.
- Changed the name of the config item `apps_per_chunk` to `appsPerChunk`.
- Changed the name of the config item `channel_count` to `channelCount`.
- Changed the name of the config item `first_snap` to `firstSnap`.
- Changed the name of the config item `snap_to_grid` to `snapToGrid`.
- Changed the name of the config item `unripe_dist` to `unripeDist`.

**chifra chunks**

- Added `chifra chunks index --tag` option to mark an Unchained Index with a version tag.
- Added `chifra chunks index --diff` to compare two versions of the Unchained Index and identify what addresses are new, missing, or modified.
- Added `chifra chunks manifest --pin [--remote]` option to pin the Unchained Index either locally, remotely, or both.
- Added `chifra chunks manifest --rewrite` option to rewrite the newly pinned manifest to the Unchained Index folders.
- Added `chifra chunks manifest --publisher` option to allow for downloading alternatively published versions of the index.
- Added `chifra chunks pins --list` option to list existing pins on either the local or remote pinning service.
- Added `chifra chunks pins --unpin` option to remove a list of unused pins from the local or remote pinning service.

**chifra init**

- Improved `chifra init` in various ways including more accurate downloading and better reporting.
- Added `chifra init --publisher` option to allow for downloading alternatively published versions of the index.
- Added progressive backoff when the download is rate-limited to avoid banning.

**chifra explore**

- No changes.

**chifra slurp**

- Added `chifra slurp --withdrawal` option to download and show withdrawal transactions.
- Added `chifra slurp --ether` to display balances and ammounts in Ether.

**makeClass**

- Various changes to support different auto-generation code.

**testRunner**

- No changes.

## Pull Requests (95)

<!-- gh pr list --search "is:pr is:closed closed:>2023-08-13" --limit 300 --state merged | cut -f1,2 | sed 's/^/- #/' -->

- #3378 Final changes to make updated index possible
- #3375 Very small update to tracing rpc usage to corrent a very small issue
- #3374 Further improvements 2
- #3373 Removes history group from toml file.
- #3372 Further improvements including fix to #3363
- #3370 Various small improvements
- #3368 3299 chifra scrape touch
- #3367 3307 chifra monitors should have a decache option
- #3366 Display type field in transactions (if non-zero)
- #3364 Improved chifra index manifest
- #3362 Allows for adjustible sleep time during pinning based on size.
- #3360 Implements chifra export --withdrawals
- #3356 Bump google.golang.org/grpc from 1.54.0 to 1.56.3 in /src/apps/chifra
- #3355 Fixes the issue further
- #3354 Fixes an issue with finding block with no addresses (which is actuall…
- #3353 Bump google.golang.org/grpc from 1.54.0 to 1.56.3 in /src/apps/chifra
- #3352 3324 chifra chunks index truncate should show progress
- #3351 chifra init --all overwriting valid chunks
- #3349 Fix/erigon trace change
- #3345 Adds --withdrawal to various
- #3344 Removes unused --wei global option
- #3341 Hot fixes two issues with version and withdrawal txs being in index.
- #3337 Typos
- #3336 Bump google.golang.org/grpc from 1.54.0 to 1.56.3 in /src/apps/chifra
- #3335 Various improvements to various things
- #3334 Feature/chunks pins count 1
- #3328 Fix chifra chunks index --check for chifra init mode
- #3327 Hot fix to allow for upgrading to 1.5.0 while still allowing downgrades
- #3326 Feature/withdrawals
- #3320 Fixes slow scraper?
- #3318 Partial fix for issue #2928
- #3317 Updates boxo
- #3316 Fixes issue #3313
- #3311 Removes most boilerplate for IsPorted and handled from output.go thro…
- #3309 Fixes tagging
- #3308 Allows for managing remote pins and tagging the index
- #3303 No longer deletes chunks ahead of manifest in chifra init -all
- #3300 Re-writes chifra init --dry_run
- #3297 Adds --rewrite option to chifra chunks --pin --deep
- #3296 Tries to fix testing on develop
- #3295 Improves debugging of logs and index
- #3292 Improvements to chifra names and closes various issues.
- #3285 Closes chifra abis - --clean option should be called --decache #3284
- #3283 First pass at notification system
- #3281 Fixes issue chifra init if it changes any files -- should invalidate …
- #3280 Bumps version to v1.5.0-release
- #3279 Fix/remote manifest
- #3278 Fix/3272 file option broken
- #3277 chifra chunks --pin improvements
- #3276 Finished this super painful merge
- #3275 Cleaning
- #3273 Cleans up a few test cases and temporarily turns off chifra monitors
- #3271 New scraper and many other things
- #3270 Various updates to pinning while scraping (removed), withdrawals on blocks (added), other - slight breaking change
- #3269 Fixes build
- #3265 Further cleanup in preparation for version 2.0.0
- #3262 Fix/build fixes
- #3261 Adds chain to Model interface
- #3249 Bump get-func-name from 2.0.0 to 2.0.2 in /sdk/typescript
- #3244 New scraper - pre-merge cleanup 2
- #3243 New scraper - pre-merge cleanup 1
- #3237 Implements chifra config edit
- #3235 Fixes issue #3232
- #3231 Closed issue #2964
- #3230 Closed issue chifra scrape do not to run twice against the same chain…
- #3228 Fixes issue #3014
- #3203 Overridden MaxIdleConnsPerHost to fix infinite bind
- #3200 Removes old files
- #3197 Fix/fix scraper 3
- #3194 Fix/fix scraper 2
- #3192 Fixes previously broken IsNodeTracing for non-mainnet chains
- #3190 Slightly better reporting for chifra chunks
- #3188 Fixing big int usage
- #3187 Fixes #3081
- #3186 Closed #3033
- #3184 Fixes #3156
- #3182 3180 chifra sdks links
- #3178 Better/chifra when
- #3177 Fixes #2862
- #3176 2849 chifra monitors watch
- #3175 Removed previously deprecated stuff
- #3172 Moved protobufs compilation step to make generate
- #3171 Bump vite from 4.1.4 to 4.4.9 in /sdk/typescript
- #3170 Bump word-wrap from 1.2.3 to 1.2.5 in /sdk/typescript
- #3169 Bump semver from 6.3.0 to 6.3.1 in /sdk/typescript
- #3168 Bump requests from 2.27.1 to 2.31.0 in /sdk/python
- #3167 Update to v1.0.0
- #3166 Bugfix/migration typo
- #3165 Removes big dogs
- #3163 Post merge cleanup 1
- #3162 Fixes issue #2810
- #3160 Fixes issue #3159
- #3155 Feature/final golang port 2

## Issues Closed (110)

<!-- gh issue list --search "closed:>2023-08-13 is:closed is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/' -->

- #3369 chifra cmd - make this easy one line change
- #3365 chifra chunks manifest --pin could be concurrent
- #3363 chifra init
- #3342 chifra cmd - add --ether to blocks, logs, traces, withdrawals
- #3340 chifra slurp - is there withdrawal data from Etherscan?
- #3333 chifra chunks - manifest --pin allows for invalid manifest
- #3331 cmd chunks - improvements to pins --unpin and pins --list
- #3330 chifra cmd - pinata
- #3324 chifra chunks - index --truncate should show progress
- #3323 chifra scrape - sleep for six seconds which is the expected time before the next block
- #3322 chifra scrape - better display
- #3321 chifra scrape - improved bloom filters
- #3319 chifra chunks - additional --deep checks
- #3315 chifra chunks - pins --unpin needs a --diff option to compare pinned items to the latest manifest and report on only orphans
- #3314 chifra chunks - manifest --pin should update the manifest by default and update all with --deep
- #3313 chifra chunks - pins --list should allow for 'all | pinned | unpinned | pending'
- #3312 chifra export - reconciliations should be export in an industry standard format
- #3307 chifra monitors should have a --decache option
- #3306 Quick look at JSON-RPC batching
- #3304 chifra chunks - reminder - test cases marked as `index`
- #3301 Additional `export` command examples
- #3299 chifra scrape - starting at a given block
- #3298 chifra init - should do better reporting for --dry_run
- #3291 chifra names - .eth should always resolve to an address if the name exists
- #3290 chifra names - incorporate into other tools
- #3289 chifra names - quality of the data
- #3288 chifra names -- names database should not be part of the repo
- #3287 chifra names - ERC20 tokens that are proxied are not identified as ERC20
- #3284 chifra abis - --clean option should be called --decache
- #3274 Settings for running a partial archive node?
- #3272 chifra export --file fails if file has no header
- #3260 chifra cmd - we need chain in the Model function for per-chain data
- #3259 chifra serve - should report error if port 8080 is being used
- #3258 chifra scrape - non-mainnet chains will not pull `withdrawals` because we hard code Shanghia block
- #3257 chifra init - if IPFS is running locally...
- #3256 What to do with dependabot warnings
- #3255 chifra chunks manifest --pin does not re-write the manifest - order of operations?
- #3254 Upgrade doesn't work
- #3253 chifra blocks - cache does not preserve withdrawals
- #3252 chifra scrape - documenting misconfigured miner records
- #3251 chifra blocks - text format output is weird with withdrawls
- #3250 chifra cmd - don't write a cache item if the file already exists
- #3248 chifra scrape - rework first_snap, apps_per_chunk defaults.
- #3247 chifra scrape - don't do the withdrawals query prior to the shanghia hard fork
- #3246 chifra cmd - reading from outdated cache should remove unreadable file
- #3242 chifra scrape - pinning options
- #3241 chifra scrape - performance
- #3240 chifra scrape - creation of manifest while scraping vs. chifra chunks --pin
- #3239 chifra cmd - no error report if tracing is not enabled on the node endpoint
- #3238 chifra scrape configuration cleanup
- #3236 chifra scrape crashes if I stop the node
- #3233 chifra monitors -- does it work?
- #3232 chifra daemon scrape values invalid
- #3229 Indexing an unsupported EVM chain
- #3227 chifra state cores
- #3226 chifra abis should have a --raw option
- #3225 Utilize BlockRange on eth_getLogs querys
- #3223 ABI docs are vague and unclear
- #3221 chifra blocks reports error incorrectly
- #3220 Multiple RPC providers
- #3219 chifra scrape needs configurable publisher
- #3216 chifra scrape or chifra init creates weird folders
- #3215 chifra tokens with --verbose produces empty dates
- #3214 chifra cmd - thoughts from making a tutorial in Berlin
- #3213 chifra export --trace --count doesn't work
- #3212 chifra cmd -- any use of `--chain` with a value not found in the array should fail more gracefully
- #3211 Omission of popular contracts with long vanity addresses
- #3210 chifra chunks manifest --pin requires ipfs even if the help text says otherwise
- #3207 chifra blocks no reporting...
- #3206 chifra when --list on other chains chokes
- #3205 Could not establish ts file: abi not found
- #3204 Cntl+C doesn't quite work in chifra scrape
- #3201 Gaps in index
- #3199 API sometimes returns blocks with empty transactions Array
- #3195 chifra scrape - misses
- #3193 Clean up BlazeOptions
- #3191 chifra cmd - IsNodeTracing return true for all chains
- #3189 Feature suggestion: Helm Chart for trueblocks-docker
- #3185 chifra scrape ignores withdrawl data
- #3183 Cleanup other folder remove old code
- #3180 chifra sdks have incorrect links in docs to old repo
- #3179 chifra scrape - Instable node connection
- #3174 Remove previously deprecated options from all tools
- #3164 Remove big dogs
- #3161 chifra tokens should show date with --verbose
- #3159 chifra list --verbose reports incorrect dates for transactions in the same block
- #3158 Auto code generation should export the VERSION file in the docker repo
- #3157 progress reporting
- #3156 chifra chunks incorrect behaviour
- #3153 chifra blocks --uncles does not cache
- #3146 Error while reading stderr: bufio.Scanner: token too long
- #3145 "No new blocks at block"
- #3143 Channel in Connection object
- #3142 Any array stored in the cache should be protected with a 'present' flag
- #3141 The use of any and interface doesn't allow us to "break" the build on purpose as a way of searching for a feature
- #3140 --decache does not report anything in API mode
- #3136 chifra tokens should have a --no_zero and a --diff
- #3135 Both chifra state and chifra tokens need to show date when --verbose flag is on
- #3134 We could probably use the `errcheck` default exclusion for the linter to avoid the `_ =` markers in the code
- #3132 Need a test for chifra X --cache should control for Control+C for the user.
- #3130 chifra blocks could easily have an --articulate options (and should)
- #3119 chifra export needs to be made IterateOverMap -- see chifra export --logs
- #3116 chifra empty hex string
- #3115 chifra status does not properly report on new cache locations
- #3106 chifra blocks Extraneous errors reported by articulation (and elsewhere)
- #3097 chifra blocks - Can `cacheTx` and `cacheTraces` be made into global capabilities?
- #3096 `--decache` should be turned on wherever `cache` is enabled.
- #3095 Remove `log_level` and `wei` throughout (anything else?)
- #3094 Deprecate global options --ether and --wei and replace with --units and --decimals which make sense for both Eth and tokens.

## Issues Opened (22)

<!-- gh issue list --search "created:>2023-08-13 is:open is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/' -->

- #3377 chifra cmd - search for "Why do we need to do this here" for a question about loading the timestamps file
- #3376 chifra chunks index --tag should update the manifest with the version and the spec
- #3361 chifra export - fake withdrawal tx is incorrect
- #3359 chifra when - finalized
- #3350 Etherscan explorer clones probably have API endpoints
- #3348 chifra state - fails against Geth
- #3347 chifra export - example miner txs that doesn't balance
- #3346 chifra cmd - all cachable items need a function NeedsUpgrade
- #3343 chifra scrape - should we be scraping access lists?
- #3338 chifra cmd - notes from call with Dawid
- #3325 chifra export -- should take dates as well as blocks
- #3310 chifra cmd - can't kill download of timestamps
- #3302 chifra transactions - Missing data in output
- #3282 chifra scrape - tracking issue for Notify
- #3267 chifra daemon --port issues
- #3266 chifra cmd - caching could possibly be a configurable option
- #3245 chifra cmd - the --publisher option should be global
- #3224 chifra abis - Separate ABIs Response instead of Union
- #3222 chifra export should handle withdrawals
- #3217 chifra cmd - every command wants to download timestamps
- #3209 chifra status - Improve UX of `chifra status` Progress row
- #3173 chifra state - Allow following proxy in `chifra state --call`

## v1.0.0 (2023/08/20)

**OUR FIRST OFFICIAL RELEASE!**

This is our first official 1.0.0 release, which means we are feature complete and we will no longer be creating backwards-incompatible changes that will break your existing code (without providing a viable migration path). The release contains a HUGE amount of changes to version v0.70.0 which happened about four months ago. The changes are detailed below. This release requires a pretty draconian migration (you'll have to remove some of your existing caches whose formats have changed). This is the last time, we'll have such a heavy migration.

Make sure to follow the [migration](./MIGRATIONS.md) (it's simple, but does remove your caches).

The most important improvements with version 1.0.0 are:

1) All of our code is now in GoLang. This means that nearly all of our code is concurrent, which means we're seeing nearly 20-30 time speedups in some code paths, and
2) Caching is now more complete. Previously, we cached only transactions and some few other things, now nearly every data structure can be cached (by adding the `--cache` option to a command). Also, every command that has a `--cache` option, now has a `--decache` option to remove those items from the cache.

Below is an explicit list of all changes.

## Specification

There were no changes to the [Specification for the Unchained Index](https://trueblocks.io/papers/2023/specification-for-the-unchained-index-v0.51.0-beta.pdf) since the last release.

## Breaking Changes

The following breaking changes were made for this release:

- Bumped version number to v1.0.0.
- Bumped required GoLang version to 1.21.x.
- Bumped required CMake version 3.5.
- The format for all of the following caches has changed: `blocks`, `transactions`, `traces`, `slurps`, `reconciliations`, and `neighbors`. If `chifra` finds any of these caches in its cache folders, it will refuse to run until you [follow the migrations](.MIGRATIONS.md).
- Removed support for `chifra export --accounting`, `chifra export --neighbors`, and `chifra transactions --account_for` options from the core package. This is a breaking change. These tools will be re-enable, but they will require a license key. More on this change later. If you are using these options, either (a) do not upgrade to this version, or (b) contact us and we will give you a complementary key (this offer has a limited lifetime at our discretion). The `chifra export --balances` option has been added, which does not require a license key.
- Numerous breaking changes to the data models (see below).
- Many breaking changes to command line "hotkeys" to make them more consistent across tools (see below).
- The removal or deprecation of some esoteric, rarely used, or redundant options (see below).
- For any command that had a `--first_record/--max_record` option, the `--first_record` (which was previously `one-based`) is now `zero-based`. This is a breaking change to any script that uses `--first_record`.
- The `--log_level` option has been removed from all tools. There is no replacement.

## System Wide Changes

- All -- every single line -- of C++ code is removed from the `chifra` tool. We are now a 100% GoLang shop!
- The `chifra daemon` command now has a new option `--grpc` which, when included, starts a GRPC server in support of any tool that uses the names database. This is a performance-related change and should have no noticeable effect on any tool's behavior (other than speedups).
- Enabled `--cache` and `--decache` on all meaningful options that query the RPC.
- Our continuous integration tooling now updates our Docker version each time we push to `master`.
- Exposed a number of previously hidden options and command (see below).
- Updated the Unchained Index manifest to the latest block.
- Improved the GoLang automatically generated documentation for all tools and all packages.
- Improved the examples and ported them to GoLang.
- Added proper GoLang linting to the automatic build process. PRs may no longer be merged into the `master` or the `develop` branch without passing linting.

## Changes to Data Models

The following existing data models were either added, removed, or modified by having their fields added to, removed, or renamed. Some models were changed extensively. Please consult the documentation on our website for details. Some of the changes were breaking.

### Modified data models

- `Transaction`: Many changes, some breaking. Consult the documentation for details.
- `Log`: Many changes, some breaking. Consult the documentation for details.
- `Trace`: A new field, `TraceIndex`, was added.
- `TraceAction`: Two new fields, `RewardType` and `Author`, were added.
- `Timestamp`: A new field, `Diff`, was added to report on changes from the previous record in the display.
- Many models:
  - We added a `Timestamp` and `Date` field to many models (i.e., `State` and `Token`). This was done in order to make the data from different tools more consistent.
  - In some cases, `Timestamp` and `Date` will only appear under the `--verbose` option. Consult the documentation.
  - For any data model with a `Timestamp`, that data model now also has an (automatically-generated) `Date` field.

### New data models:
- `ChunkPin`: Added `ChunkPin` data model. Used by the `chifra chunks` command.
- `Slurp`: Added `Slurp` data model. Used by the `chifra slurp` command.

### Remove data models
- The `MonitorCount` data model was removed as unused. Previously used by the `chifra monitors --count` command.

### Renamed data models
- `EthState` was renamed to `Result`. Used by the `chifra state --call` command.
- `Reconciliation` was renamed to `Statement`. Used by the `chifra export --accounting` commands.
- `TokenBalance` was renamed to `Token`. Used by the `chifra tokens` and `chifra export --accounting` commands.

## Tool Specific Changes

**chifra list**

- Changed the default meaning of `chifra list --first_record` from `1` to `0` for this (and all) tools. This is a breaking change.
- Removed previously `chifra list --appearances` as redundant. The same command without `--appearances` produces identical results.
- Added `chifra list --reversed` option to reverse the order of the list.
- Exposed previously hidden option `chifra list --silent` with `-s` hotkey. This updates the monitor's list of appearances but does not report anything to the output. Useful for scripting.
- Exposed previously hidden option `chifra list --bounds` with `-b` hotkey. This returns a range of blocks and timestamps for the first and last appearance of the address. Useful for summarizing the data.
- Changed hotkey for `chifra list --no_zero` from `-n` to `-z` to be consistent with other tools.

**chifra export**

**Special important note:**. Previously, `chifra export` streamed its results, which means it worked well with both small and very large addresses (such as UniSwap). With this release, due to the way our concurrent code works, we no longer stream. Now, we accumulate the results in memory prior to display. This means that, for very large addresses, you will most likely run out of memory. Instead, you must use the `--first_block/--last_block` options to limit the size of the output prior to calling `chifra`. (For the API usage, you are probably already doing this.) If you don't do this on the command line, `chifra export` will hang and probably crash. We're working on a design to fix this issue shortly.

- Changed the default meaning of `chifra export --first_record` from `1` to `0` for this (and all) tools. This is a breaking change.
- Added `chifra export --balances`. This option reports on balances for each appearance (no reconciliation) and does not require the `--accounting` option.
- Exposed previously hidden option `chifra export --factory` with `-y` hotkey. This option reports all deployments of a smart contract by the given address (including smart contract addresses such as the UniSwap factory).
- Exposed previously hidden option `chifra export --reversed` with `-E` hotkey. This option reverses the list of appearances prior to any filtering or processing. This option currently works for all options other than `--accounting`.
- Exposed previously hidden `chifra export --ether` option. Where applicable, this option reports the value of the transaction in Ether rather than in Wei.
- Exposed previously hidden `chifra export --no_zero` option (with `-z` hotkey) to remove zero-value records from `--balances` and `--accounting` reports.
- Added a hotkey `-P` to the `chifra export --asset` option.
- Added a hotkey `-B` to the `chifra export --topic` option.
- Added a hotkey `-m` to the `chifra export --emitter` option.
- Added a hotkey `-N` to the `chifra export --relevant` option.

**chifra monitors**

- Removed `chifra monitors --decache` option. Use `chifra export --decache` instead.
- Removed `chifra monitors --first_block` and `chifra monitors --last_block` as unnecessary.
- Added a hotkey `-C` to the `chifra monitors --clean` option.
- Added a hotkey `-l` to the `chifra monitors --list` option.
- Added a hotkey `-w` to the `chifra monitors --watch` option.

**chifra names**

- Exposed previously hidden `chifra names --clean`. This option will `clean` the names database by removing all names that are no longer valid.
- Exposed previously hidden `chifra names --dry_run`. This option will produce a full listing of all names in the names database that would be changed if the `--clean` option were used. Redirecting the results of this command back into the names database will update the database.
- Exposed previously hidden `chifra names --autoname`. This option, if given an address, will query the chain for token information (such as Name, Symbol, Decimals, etc.) and automatically add that information to the names database.
- Removed the `chifra names --to_custom` and `chifra names --named` options as redundant or unused.
- Changed hotkey for `chifra names --addr` option from `-a` to `-s` to be more consistent with other tools.
- Changed hotkey for `chifra names --all` option from `-l` to `-a` to be more consistent with other tools.
- Added a hotkey `-d` to the `chifra names --dry_run` option.
- Added a hotkey `-r` to the `chifra names --regular` option.

**chifra abis**

- Change hotkey for `chifra abis --clean` from `-c` to `-C` to be more consistent with other tools.
- Removed `chifra abis --sol` option. Use Go Ethereum's `abigen` instead.

**chifra blocks**

- Removed the `chifra blocks --apps` option as unused. This is a breaking change.
- Deprecated `chifra blocks --list` and `chifra blocks --list_count` options. This is a future breaking change.
- Added the `chifra blocks --raw` option. This option returns the raw RPC data retrieved from the node without modification.
- Exposes previously hidden option `chifra blocks --logs`, which displays the logs in the given block.
- Exposes previously hidden option `chifra blocks --emitter`, which may be used with the `--logs` option to filter only those emitters.
- Exposes previously hidden option`chifra blocks --topic`, which may be used with the `--logs` option to filter only those topics.
- Exposes previously hidden option`chifra blocks --articulate`, which articulates the transactional, log, or traces data in the block if possible.
- Exposes previously hidden option `chifra blocks --big_range`, which may be needed for the `--logs` option to limit the amount of data requested from the node. The node may crash otherwise.
- Added the `chifra blocks --cache_txs` option. This may be used only with the `--cache` option to additionally cache the transactions in a block. Note this is slow, but the subsequent queries for the block are greatly sped up. Off by default.
- Added the `chifra blocks --cache_traces` option. This may be used only with the `--cache` option to additionally cache the traces in a block. Note this is very slow, but the subsequent queries for the block are greatly sped up. Off by default. It may produce a very large number of cache items.
- Changed hotkey for `chifra blocks --logs --topic` option from `-p` to `-B` to be more consistent with other tools.
- Changed hotkey for `chifra blocks --logs` option from `-g` to `-l` to be more consistent with other tools.

**chifra transactions**

- Removed `chifra transactions --reconcile` as redundant with `--account_for`. This is a breaking change.
- Removes previously deprecated `chifra transactions --reconcile`. This is a breaking change.
- Added `chifra transactions --cache_traces` which will cache traces if the `--cache` option is on. Note that on first invocation, this option is quite slow, but subsequent queries for the same transactions are much faster.
- Added `chifra transactions --logs`, `chifra transactions --logs --emitter`, and `chifra transactions --logs --topic` analogous to `chifra blocks --logs` options. Use of these options is the fast path to the RPC and is encouraged.
- Exposed previously hidden `chifra transactions --ether` option. Where appropriate, this displays the value in Ether instead of Wei.
- Exposed previously hidden `chifra transactions --raw` option. This displays the unadulterated raw data returned by the RPC.

**chifra receipts**

- Enabled `chifra receipts --raw`, `chifra receipts --cache`, and `chifra receipts --decache` options.

**chifra logs**

- Enabled `chifra logs --raw`, `chifra logs --cache`, and `chifra logs --decache` options.

**chifra traces**

- Enabled `chifra traces --raw`, `chifra traces --cache`, and `chifra traces --decache` options.

**chifra when**

- Enabled `chifra when --cache` and `chifra when --decache` options.
- Exposed previously hidden option `chifra when --timestamps --check --deep`. This tool will query the node for all timestamps for all blocks to ensure that the timestamps file is accurate and complete. This is a very slow process and should only be used when there is a problem with the timestamps file.
- Added a hotkey `-u` to the `chifra when --update` option.
- Change hotkey for `chifra when --deep` option from `-e` to `-d` to be more consistent with other tools.

**chifra state**

- Exposed previously hidden `chifra state --call` option. This is a remarkably powerful new feature that allows for Solidity-like syntax for calling functions among many other things.
- Exposed previously hidden `chifra state --proxy_for` option. This is useful for the `chifra state --call` option when the proxy is not discoverable through the ABI or other means. You can explicitly tell the command where the proxied-to address is.
- Added `chifra state --articulate` option to articulate the return value for `chifra state --call` and others.
- Enabled `chifra state --ether`, `chifra state --cache`, and `chifra tokens --decache` options.
- Changed hotkey for `chifra state --call` option from `-a` to `-l` to be more consistent with other tools.
- Changed hotkey for `chifra state --no_zero` option from `-n` to `-z` to be more consistent with other tools.

**chifra tokens**

- Added `chifra tokens --changes` option to limit display to only those records where balances change between records.
- Enabled `chifra tokens --cache` and `chifra tokens --decache` options.
- Changed hotkey for `chifra tokens --no_zero` option from `-n` to `-z` to be more consistent with other tools.

**chifra config**

- Exposed previously hidden `chifra config --paths`. This is useful during migrations and other times when debugging the configuration.

**chifra status**

- Changed available cache names for the mode of `chifra status <mode>`. Previous values of `[index|blooms|blocks|txs|traces|monitors|names|abis|recons|slurps|staging|unripe|maps|some*|all]` were permitted. New permitted values are: `[index|blooms|blocks|transactions|traces|logs|statements|results|state|tokens|monitors|names|abis|slurps|staging|unripe|maps|some*|all]`. Each cache was renamed accordingly. Note that changes to the cache for version v1.0.0 are a breaking change and require a migration (see above).
- Exposed `chifta status --chains` option because we no longer display the chain configurations by default in `--verbose` mode. This is a breaking change.

**chifra daemon**

- Exposed previously hidden `chifra daemon --api`, `chifra daemon --scrape`, and `chifra daemon --monitor` tools.
- Added an option `chifra deamon --grpc` to start a GRPC names server (and other data models in the future) that should speed up some queries. This is optional and experimental, so use with caution.

**chifra scrape**

- Exposes previously hidden `chifra scrape --start_block` option. Experimental. Use with caution.
- Changed hotkey for `chifra scrape --remote` option from `-m` to `-r` to be more consistent with other tools.

**chifra chunks**

- User can no longer use both `chifra chunks --pin` and `chifra chunks --publish` at the same time. This is a breaking change.
- Changes `chifra chunks [index|manifest] --pin` to only pin chunks and bloom filters if the `--deep` option is included, otherwise these commands only pin the timestamps database (both locally and remotely if `--remote` is included) and the manifest (both locally and remotely if `--remote` is included).
- Added `chifra chunks index --check --deep` option, which opens each chunk and checks that all addresses in that chunk report "not-no" when tested against the node software. This is a very slow process.
- Added `chifra chunks manifest --check --deep` which verifies that all chunks in the manifest are locally pinned and have the correct IPFS hash.
- Added a new option `chifra chunks --deep` with hotkey `-d` that is used in various places.
- Added `chifra chunks index --check --deep` to chifra chunks used `d` for hot key which used to be for `--max_addrs`
- Changed hotkey for `chifra chunks --max_addrs` hotkey changed to `-m` from `-d`.
- Changed hotkey for `chifra chunks --remote` hotkey change to `-r` from `-m`
- Changed hotkey for `chifra chunks --max_addrs` from `d` to `m` which used to be for `--remote`
- Changed hotkey for `chifra chunks --remote` from `m` to `r`

**chifra init**

- Fixed some issues with `chifra init` by adding `--dry_run` and implementing `--first_block`. `--dry_run` may be used to check what would be changed if the option was not present. After running `chifra init`, always run `chifra chunks index --check` to check the results.
- Fixes a bug in which hitting `Cntl+C` during download of the timestamps file would corrupt the database.

**chifra explore**

- No changes.

**chifra slurp**

- Enabled `chifra slurp --raw`, `chifra slurp --cache`, and `chifra slurp --decache` options.

**makeClass**

- No changes.

**testRunner**

- No changes.

## Pull Requests (46)
- #3154 Catching up to a lot of cache related code
- #3152 Starting to turn on accounting again
- #3151 Stops calling reconcile if not relevant log
- #3150 Catches up to include fixes for chifra slurp
- #3149 Touching export
- #3148 Fixes query package
- #3139 Feature/decache for all
- #3138 Update cmake
- #3137 Move token package to rpcClient, removes separate Token type in favor of Token
- #3131 Moved linter job to build workflow
- #3124 Removes chain from most methods on rpcClient.Options
- #3129 concurrent access to map core dumps
- #3121 Catches up to caching code in full port
- #3120 Bugfix/token oldbugs
- #3117 Updates chifra export to use new caching code - but still not active …
- #3113 Begins forcing access to the RPC through an object
- #3114 Bumps version to v0.76.0-beta
- #3112 Fixes incorrect use of UnchainedIndex for non-mainnet chains
- #3111 Feature/improve rpc client package
- #3110 Removes separate node package by moving it into rpcClient
- #3109 Removes separate ens package by moving it into rpcClient
- #3108 Feature/remove log level
- #3105 Adds TB_OVERRIDE_CID to force downloading a different manifest
- #3104 Fixing build
- #3090 Moved DefaultRpcOptions out of internal/global
- #3102 Adds support for chifra export --traces
- #3101 Feature/port export testing
- #3100 Add support for chifra export --receipts
- #3099 Bumps version to v0.76.0-beta
- #3098 Adding --neighbors (but disabled) to chifra export
- #3093 Adds more tests, shows caps in tests, #3092
- #3089 Bugfix/3078 maxgas not cached
- #3091 Adds caps testing
- #3088 Cleaning tests
- #3087 Move all global options to capabilities. Add Default option and imple…
- #3085 Feature/capabilities for global options
- #3084 Ports chifra export --balances to goLang
- #3083 Ports chifra export --logs to goLang
- #3062 Feature/port chifra export appearances
- #3080 Cleanup/linting with staticchecker
- #3068 Feature/finalized cache rpc options
- #3074 Cleanup/change cache help line
- #3064 Post merge cleanup
- #3057 Feature/txs enable cache
- #3125 Added workflow to trigger Docker build
- #3155 Feature/final golang port 2

## Issues Closed (37)

- #2124 chifra scrape should be able to index from a given block
- #2128 Interesting use of Waku (formerly Whisper)
- #2089 Interesting
- #1891 chifra export --first_block is slower than it needs to be in chifra export
- #1629 chifra export user comments on acctExport Traversers
- #3009 reth is not being detected as an archive node
- #3007 chifra cmd - automate cleanup of command line args
- #3010 chifra traces - we could implement the same thing Sourcify does with the init code (which we get from traces).
- #3020 chifra state crashes with null pointer
- #3024 Checklist for caching
- #3034 Function names
- #3041 Improve articulation calling code across multiple tools.
- #3038 Make sure to document the fact that topic and relevant filters now work on most data types in chifra export
- #3047 Reading from cache should happen even without --cache option
- #3045 Caching and non-final blocks near the head
- #3134 We could probably use the `errcheck` default exclusion for the linter to avoid the `_ =` markers in the code
- #3065 Better interface for all RPC queries
- #3073 It appears --cache option does not actually work in API mode
- #3075 Decache should be global just like cache and it should be available to any command that allows caching
- #3130 chifra blocks could easily have an --articulate options (and should)
- #3136 chifra tokens should have a --no_zero and a --diff
- #3140 --decache does not report anything in API mode
- #3078 chifra blocks does not cache maxGasPer...
- #3082 Cache issues
- #3115 chifra status does not properly report on new cache locations
- #3096 `--decache` should be turned on wherever `cache` is enabled.
- #3153 Remove `log_level` and `wei` throughout (anything else?)
- #3153 chifra blocks --uncles does not cache
- #3157 progress reporting
- #3144 Pending testing seconds against millseconds
- #3126 Access to Topic[0] even when len(log.Topics) == 0
- #3128 Concurrent access to map core dumps
- #3092 finishing caps
- #3079 Use bitflags where possible for capabilities
- #3086 Deprecate global options `--ether` and `--wei` and replace with `--units` and `--decimals` which make sense for both Eth and tokens.
- #2273 Add instructions for adding command line options to a tool
- #2262 Merge readiness

## Issues Opened (45)

- #3157 progress reporting
- #3156 chifra chunks incorrect behavior
- #3153 chifra blocks --uncles does not cache
- #3146 Error while reading stderr: bufio.Scanner: token too long
- #3145 No new blocks at block
- #3143 Channel in Connection object
- #3142 Any array stored in the cache should be protected with a 'present' flag
- #3141 The use of any and interface doesn't allow us to "break" the build on purpose as a way of searching for a feature
- #3140 --decache does not report anything in API mode
- #3136 chifra tokens should have a --no_zero and a --diff
- #3135 Both chifra state and chifra tokens need to show date when --verbose flag is on
- #3134 We could probably use the `errcheck` default exclusion for the linter to avoid the `_ =` markers in the code
- #3132 Need a test for chifra X --cache should control for Control+C for the user.
- #3130 chifra blocks could easily have an --articulate options (and should)
- #3122 Omission of withdrawal appearances in spec
- #3119 chifra export needs to be made IterateOverMap -- see chifra export --logs
- #3118 Decache should remove empty folders
- #3116 chifra empty hex string
- #3115 chifra status does not properly report on new cache locations
- #3107 Diagram inversion in paper
- #3106 Extraneous errors reported by articulation (and elsewhere)
- #3097 Can `cacheTx` and `cacheTraces` be made into global capabilities?
- #3096 `--decache` should be turned on wherever `cache` is enabled.
- #3095 Remove `log_level` and `wei` throughout (anything else?)
- #3094 Deprecate global options --ether and --wei and replace with --units and --decimals which make sense for both Eth and tokens.
- #3082 Cache issues
- #3081 Once we start writing a cache file, we must finish. We have to capture control+c
- #3078 chifra blocks does not cache maxGasPer...
- #3077 Tools that have `--cache_txs` or `--cache_traces` include internal package in imports.
- #3076 chifra blocks --uncles after 1559 fork can return 0 immediately.
- #3075 Decache should be global just like cache and it should be available to any command that allows caching
- #3073 It appears --cache option does not actually work in API mode
- #3072 Treasure Trove
- #3071 Run tests with TEST_SLURPS=true before releasing v1.0
- #3069 Non-caching chifra blocks is slow
- #3065 Better interface for all RPC queries
- #3063 `transactions/handle_show` loops twice through the same set of items
- #2973 chifra cmd ordering of fields in JSON
- #2959 chifra cmd we need test cases to protect against breaking changes in the RPC data
- #2964 Long ago hack needs resolution
- #2966 chifra blocks --logs -big_range is ignored
- #2969 chifra blocks --logs --articulate (and transactions --logs --articulate) do not articulate
- #2986 chifra state --call option -- allow for caller to include parameter types and return types in the call string
- #2987 chifra state --call allow for calling functions with tuple parameters and/or returns
- #2991 chifra export - reconciliation data model

## v0.70.0 (2023/05/26)

This release includes a major re-write of the `chifra names` module as well as other less obvious fixes and improvements. It also includes an important correction to the Unchained Index data stored on the smart contract which requires a migration. Make sure to follow the [migration](./MIGRATIONS.md) (don't worry -- it's simple).

In addition to improvements to `chifra names`, the primary area of improvement was in `chifra chunks`. These changes included much better support for debugging the Unchained Index as well as an ability to `--pin` the Unchained Index with `chifra chunks manifest --pin`. This command will now pin the index either locally on your own IPFS node (if it's running) or remotely (if you have properly configured the API key).

Additionally, the required GoLang version was bumped to 1.20.X. This is a breaking change to the build.

Below is an explicit list of all changes.

## Specification

There were no changes to the [Specification for the Unchained Index](https://trueblocks.io/papers/2023/specification-for-the-unchained-index-v0.51.0-beta.pdf) since the last release.

## Breaking Changes

- Bumped TrueBlocks version to v0.70.0.
- Bumped required GoLang version to 1.20.X.
- Corrections were made to the previous version of the Unchained Index -- this correction requires a migration.
- The `abiSource` field was removed from the `Function` data model.

## System Wide Changes

- GoLang version 1.20 is now required to build TrueBlocks.
- There was a correction made to the Unchained Index at and after block 15,300,000 - this requires you to re-run `chifra init -all` or a re-scrape of the entire index. See migration notes.
- Many cleanups across multiple packages. Lots of smoke, no real fire.
- Command line now removes duplicate `modes`, `addresses`, `block ids`, or `transaction ids` for some commands.
- The documentation was updated and improved.
- Certain C++ testing code that was no longer needed was removed. (The port is almost finished.)

## Changes to Data Models

- `Function`:
  - Removed `abiSource` field. If this causes problems with your cache (it shouldn't), you may simple remove the `txs` and `blocks` caches from your installation. They will be re-built.
- Various `chifra chunks` related data models were expanded or improved. Mostly internal.

## Tool Specific Changes

**chifra list**

- Added the `--unripe` option which now reports up transaction lists up to the latest block.
- Enabled better output in verbose mode including providing additional data fields such as `date`.

**chifra export**

- Removes duplicate addresses from command line if present.
- Cleans up reconciliation of "bogus" token transfers (i.e., token transfer events without a corresponding change in balance). Fixes previously mis-reported failed reconciliations.

**chifra monitors**

- Adds the `--decache` option which removes all associated data for a monitored address from any cache. (Useful for debugging.)
- Minor improvements to `chifra monitors <address> --watch` to better follow the head of the chain.

**chifra names**

- Near complete re-write of the `chifra names` module including addition of `--grpc` option when starting `chifra daemon`. This allows for the names database to be kept in memory even when used from the command line. Should improve speed of this command.
- Implemented previously-disabled `chifra names --clean`.
- Added `chifra names --clean --dry_run` which reports potential changes to the screen without changing the database.
- Added `chifra names --clean --regular` which operates on the `regular` names as opposed to the default `custom` names.
- Implemented previously-disabled `chifra names --autoname`.
- Updated the names database with newly cleaned data.

**chifra abis**

- No changes.

**chifra blocks**

- Removes little-used optional fields from display (they were never part of the actual cached data): `datesh`, `time`, `day`, `week`, etc. These fields are easily calculated using regular Linux command line tools such as `cut`.

**chifra transactions**

- Removes little-used optional fields from display (they were never part of the actual cached data): `datesh`, `time`, `day`, `week`, etc. These fields are easily calculated using regular Linux command line tools such as `cut`.

**chifra receipts**

- No changes.

**chifra logs**

- No changes.

**chifra traces**

- No changes.

**chifra when**

- No changes.

**chifra state**

- Allow for using dates in command line in order to be consistent with other tools.

**chifra tokens**

- No changes.

**chifra config**

- No changes.

**chifra status**

- No changes.

**chifra daemon**

- Added the `--grpc` option. See notes under `chira names` above.

**chifra scrape**

- Fixed an issue with the scraper that was causing it to get stuck when near the head of the chain.

**chifra chunks**

- Full support for `--pin` option in `chifra chunks index`. Tested and ready to go.
- Added `--first_block`, `--last_block` and `--max_addrs` to aide in debugging.
- Many internal updates to make debugging / editing the index easier. (Editing the index is rare, but now it's easier.)

**chifra init**

- Adds `chifra init --dry_run` (not implemented).

**chifra explore**

- No changes.

**chifra slurp**

- No changes.

**makeClass**

- No changes.

**testRunner**

- No changes.

## Issues Closed (3)

- #2915 - chifra chunks overview issue
- #2922 - Build fails on Fedora using Clang 16.0.3
- #2916 - chifra chunks future enhancements

## Issues Opened (10)

- #2934 - chifra export phony transfers
- #2933 - chifra names when started with --grpc, make test-all fails on pre-existing already customized custom names file
- #2932 - chifra export does not cache when running in Explorer
- #2931 - chifra names --autoname when used by Explorer (or command line) writes to names.tab
- #2930 - auto code generation allows for hidden options that are documented in the API
- #2928 - chifra chunks overview version 2
- #2927 - chifra chunks --publish summary of issues
- #2926 - Build proto buffers only if instructed with `make protos`
- #2925 - Reopen TA-Delayed issues
- #2920 - Add http proxy setting for etherscan request.

## v0.64.0 (2023/05/07)

This is a minor release that closes a few random issues and adds a small feature to better manage the caches.

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
- [#2848 - chifra status -- saving old behavior pre-port to golang](https://github.com/TrueBlocks/trueblocks-core/issues/2848)
- [#2847 - chifra export creates huge number of abi files when scraping UniSwap contracts](https://github.com/TrueBlocks/trueblocks-core/issues/2847)
- [#2846 - chifra monitors -- add deleted status to the fields](https://github.com/TrueBlocks/trueblocks-core/issues/2846)
- [#2845 - chifra monitors -- add --count option](https://github.com/TrueBlocks/trueblocks-core/issues/2845)
- [#2844 - chifra config - add --chains option](https://github.com/TrueBlocks/trueblocks-core/issues/2844)
- [#2843 - chifra transactions (and others) should export `nonce` field.](https://github.com/TrueBlocks/trueblocks-core/issues/2843)
- [#2841 - how to integrate gRPC types](https://github.com/TrueBlocks/trueblocks-core/issues/2841)
- [#2839 - golang IsNodeArchive always returns true. should not](https://github.com/TrueBlocks/trueblocks-core/issues/2839)
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
- Removed the global `--dollars` option from tools that still presented it. Its implementation was unsupportable. To reproduce the behavior, use the `spotPrice` exported by reconciliations.
- During articulation, the export previously included certain whitespace characters direclty (`\t`, `\r` and `\n`, for example). This was causing various problems (for example, trying to import into CSV into MS Excel). The code now replaces those characters with `[t]` for `\t` or `[n]` for `\n`, changes `\r` into a space and (in some cases) changes `,` internal to a field into `_`. This may be a breaking change depending on your usage.
- Previously, if one exported traces to CSV or TXT, and smart contracts were both created and self destructed in a single trace (which happens frequently), the code used to report the newly-minted smart contract in the `from` field of the trace. This was not canoncial. We stopped doing it. If you wish to see the address of the newly-created contract in such a case, export to `--fmt json`. The value is presented as received from the RPC.

## Bug Fixes

- There were a number of fairly minor bug fixes including one to PetNames and the above mentioned correction to the index due to Erigon's bug.

## System Wide Changes

- We completed a full port to GoLang for `chifra names`, `chifra abis`, and `chifra receipts`. In some cases, this changed the format of the output (especially for JSON output). In every case, we think the data has been improved.
- We completed partial ports for `chifra blocks`, `chifra transactions`, and `chifra traces`. In some cases, this changed the format of the output (especially for JSON output). In every case, we think the data has been improved.
- Implemented `--articulate` across many tools in GoLang. (Thanks Dawid!)
- We made significant improvements to the documentation including more examples for the API docs and cross links to data models from tools producing the same.
- We prepared all tools for using the GoLang `--cache` options (caching is not yet enabled in the GoLang code). (Thanks Dawid!)
- Better support for streaming output to various formats (including preliminary support for `.xlsx`).
- Begun improvements for more useful and flexible connections to the RPC.

## Changes to Data Models

- `Function` data model:
  - Changed `abi_source` to `abiSource`.
  - Changed `input_dicts` to `inputDicts`.
  - Changed `output_dicts` to `outputDicts`.
  - Removed `input_names` and `output_names`. (These may be added back in the future.)
- `Reconciliation` data model:
  - Changed `prevBlock` to `prevAppBlk`.
  - Changed `prevBlkBal` to `prevBal`.
  - Changed `getCostOut` to `gasOut`. (This may be renamed in the future.)
  - Added `accountedFor` field.
- `Block` data model:
  - Removed `unclesCnt`.
- `TraceResult` data model:
  - Renamed `newContract` to `address` in order to agree with the RPC.
- Renamed `VerboseAppearance` data model to `Appearance` to be consistent with other tools.
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
- Added placeholder option called `--source` which meant to trace the source of funds into an address. Currently not implemented.

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
- Separation of `CParameter` class from `CMember` class making publically presented `CParamater` much simpler since most of the complications came from that class's use in makeClass.

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
- First introduction of very rudimentary TypeScript and Python SDKs (currently pre-alpha and only supported for internal use).
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

- An attempt was made to improve the data exported from this tool, as it was quite confused previously. There may be unforeseen breaking changes to the exported data.
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

- Added `chifra version` which is identical to many other command line tools. Identical behavior to `chifra --version` which still works.

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
  - *Breaking change:* `--accounting` no longer turns `--articulate` on by default (users must explicitly add `--articulate` to get same behavior).
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
