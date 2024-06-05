# History of Changes

This file details changes made to TrueBlocks over time. All version prior to version 3.0.0 are now no longer supported.

## v3.0.0-alpha (2024/06/01)

**Summary**

- Many, many changes, some breaking.
- Most important was the addition of a complete GoLang SDK (still alpha, but working).
- Major improvements and expansions to caching.

## Changes to the Unchained Index Specification

- No changes.

## Breaking Changes

- Removes the word `Simple` from all data types.
- Removes all `Raw` data types.
- In general, when `--ether` option is used we no longer replace the `value` field. Instead, the `value` field remains in WEI and a new field called `ether` is added. (Applies to multiple commands.)
- See below for other breaking changes.

## System Wide Changes

- Adds a GoLang SDK. Major new feature. Makes programming local-first, desktop applications in GoLang MUCH easier.
- Updates Python and Typescript SDK to latest auto-generated code.
- Extended caching to various commands including `chifra state`, `chifra receipts`, and `chifra export --accounting`.
- General improvements to caching including the ability to upgrade the cache without requiring a migration.
- Moves `MetaData` type to the `types` package out of the `rpc` pacakge.
- Removes `go.work` from the repo which now requires running `./scripts/go-work-sync.sh` as part of the build.
- Moved the test cases into its own submodule. Build now requires `git pull --recurse-submodules` to get the tests.
- Complete rewrite of `goMaker` and `testRunner` in GoLang. Totally removes old `cpp` code.
- Moves VERSION file into root of repo making bumping the version much easier.
- Adds `sdkFuzzer` which calls each of the SDK endpoints with all combinations of options.

## Changes to Data Models

The following data models were either modified, added, removed, or renamed by having their fields added to, removed, or renamed. Please consult the documentation on our website for details.

### Modified data models

| model       | description                                                                                             |
| ----------- | ------------------------------------------------------------------------------------------------------- |
| Block       | Makes `Block` type a non-generic                                                                        |
|             | Marks `baseFeePerGas` as base type `gas` to be more accurate.                                           |
|             | Marks `difficulty` as a `value` type to be more accurate.                                               |
| Bounds      | Changes `firstApp` and `latestApp` from string to `Appearance` to make parsing as JSON easier.          |
| Log         | Makks `compressedLog` as calculated field.                                                              |
|             | Marks `logIndex` as `lognum` type as opposed to `uint64` as a more accurate representation.             |
|             | Marks `transactionIndex` as `txnum` type as opposed to `uint64` as a more accurate representation.      |
| Receipt     | Marks `status` field as type `value` as opposed to `uint32` to be more accurate. (Modifies cache data.) |
|             | Marks `transactionIndex` as `txnum` type as opposed to `blknum` as a more accurate representation.      |
| ReportCheck | Converts all fields that were previously `uint32` to `uint64`.                                          |
| Slurp       | Marks `compressedTx` as calculated                                                                      |
| State       | Marks `nonce` as `value` as a more accurate representation.                                             |
| Statement   | Changes `decimals` field from `uint64` to `value`.                                                      |
|             | Changes `logIndex` from `blknum` to `lognum` to be more accurate.                                       |
|             | Changes `spotPrice` field from `double` to `float64`.                                                   |
|             | Changes `transactionIndex` from `blknum` to `txnum` to be more accurate.                                |
| Token       | Marks `diff` as a calculated field.                                                                     |
|             | Marks `transactionIndex` as a `txnum` type as a more accurate representation.                           |
| Trace       | Marks `compressedTrace` as calculated field.                                                            |
|             | Marks `transactionIndex` as `txnum` type as opposed to `uint64` as a more accurate representation.      |
| Transaction | Marks `compressedTx` as calculated field.                                                               |
|             | Marks `hasToken` and `isError` as `bool` as more accurate.                                              |
|             | Marks `nonce` as a `value` type as opposed to `txnum` as a more accurate representation.                |
|             | Marks `transactionIndex` as a `txnum` type as a more accurate representation.                           |
| Withdrawal  | Marks `index` field as type `value` to be more accurate.                                                |
|             | Marks `validatorIndex` field as type `value`to be more accurate.                                        |

### New data models

| model      | description                                                            |
| ---------- | ---------------------------------------------------------------------- |
| LightBlock | New type to represent `Block` data with hashes of `Transactions` only. |

### Removed data models

| model           | description        |
| --------------- | ------------------ |
| SlurpCount      | Previously unused  |
| AppearanceCount | Removed as unused. |

### Renamed data models

| model    | description                                            |
| -------- | ------------------------------------------------------ |
| ChunkPin | Renamed this field from `ChunkPinReport` to this name. |

## Tool Specific Changes

**chifra list**

- No changes.

**chifra export**

- Breaking: `chifra export --ether` option used to replace the `value` field when present. Now, `value` field is left where it is and a new field (called `ether`) is provided.
- Breaking: `chifra export --bounds` option now produces an `Appearance` type for `first` and `last` appearances. It used to produce an unparsable string.
- `chifra export --decache` option now cleans up `Receipts` and `Withdrawal` caches. (It used to leave these items in place.)
- Removes previously deprecated and unused `chifra export --load` option.
- Removed the global `chifra export --raw` option as unused.

**chifra monitors**

- Added `chifra monitors --decache` option which removes all data types produced by the `chifra export` command for the given address including ABIs and the monitor itself.
- Added `chifra monitors --watch --run_count` option to limit the number of runs to process (aids with testing).

**chifra names**

- Corrects the process of some names that contained UniCode strings.

**chifra abis**

- `chifra abis --find` now shows properly formatted output (json, txt, or csv). Previously always showed TXT output.

**chifra blocks**

- Breaking: When `chifra blocks --ether` option is provided, a new field (`ether`) is provided. Previously the `amount` field was replaced with the `ether` field. Both are now provided.
- Added `chifra blocks --cache_txs` to explictly cache transactions. Previously, only transaction hashes were cached (as they still are).
- Added `chifra blocks --cache_traces` to explictly cache transactions at user's discretion.
- Removed `chifra blocks --big_range`, `chifra blocks --list`, and `chifra blocks --list_count` as unused.
- Removed the global `--raw` option as unused.

**chifra transactions**

- Added `chifra transactions --cache_traces` to explicitly cache traces if the `chifra transactions --traces` option is used.
- The `--ether` option previously displayed an Ether value under the `value` key. Now, the `value` key continues to display WEI value and a new key called `ether` carries the Ether value.
- Removed previously unimplemented `--seed` option.
- Removed the global `--raw` option as unused.

**chifra receipts**

- Breaking: `cumulativeGasUsed` is now reported as an integer instead of big number.
- The `to` and `from` fields are now included in output.
- Removed the global `--raw` option as unused.

**chifra logs**

- JSON output now includes `timestamp` and `date` keys.
- Removed the global `--raw` option as unused.

**chifra traces**

- Breaking: the `Result` key of the output now returns empty object (`{}`) as opposed to `null` in JSON output.
- Breaking: the `Action` key of the output now returns empty object (`{}`) as opposed to `null` in JSON output.
- The `--ether` option previously replaced the `value` column with `ether` for CSV and TXT output.  Now, it leaves `value` column in place and adds `ether` column to output.
- Removed the global `--raw` option as unused.

**chifra when**

- Added additional special blocks for `firstLog` and the `Dencun hard fork`.

**chifra state**

- Removes the `none` option from `chifra state --parts` enum.
- If `--ether` option is provided, now shows `balance` field (in WEI) in addition to `ether` field. Used to retplace the `balance` field with a new field called `ether`. Both are shown now.
- Enables cache for both `chifra state` and `chifra state --call`. Previously disabled.

**chifra tokens**

- The `balance` key used to carry decimal balances, it now carries what was previously called `units` so as to agree with the value returned from the RPC.
- The key previously called `units` is now called `balanceDec` (balance in decimals).
- Added a `some` option to `chifra tokens --parts` enum.

**chifra config**

- No changes.

**chifra status**

- No changes.

**chifra daemon**

- Adds `chifra daemon --silent` option to allow starting the daemon without producing log data. This allows the SDK to start to the daemon and handle its own logging.
- Removes previously unused `chifra daemon --fmt` option.

**chifra scrape**

- Added the `chifra scrape --notify` feature that notifies listening servers of new appearances at each block and new chunks at each consolidation. Requires the `--notify` option and configuration data to enable.
- Removed the global `chifra scrape --raw` option as unused.
- Added `chifra scrape --run_count` option to limit the number of runs to process (aids with testing).

**chifra chunks**

- No changes.

**chifra init**

- Added `chifra init --examples <name>` option which creates a folder with a simple SDK example file ready to run.

**chifra explore**

- No changes.

**chifra slurp**

- Breaking: Deprecated `chifra slurp --types` option in favor of `chifra slurp --parts` to be consistent with other endpoints.
- Adds `chifra slurp --source` option which allows specification of the remote data provider: `etherscan`, `key`, `covalent`, or `alchemy`.
- Exposes previously hidden `chifra slurp --page`, `chifra slurp --page_id`, and `chifra slurp --per_page` option.
- Adds `--page_id` option for better pagination for some providers.
- Previously, `--ether` option would replace `value` with `ether`. Now shows original WEI as gotten from the endpoints plus `ether` field.
- Adds `some` option to `chifra slurp --parts` enum.
- Removed the global `--raw` option as unused.

**makeClass**

- Complete rewrite in GoLang.
- Consolidates all auto-code-gen templates from to `./src/dev_tools/goMaker/templates`.
- Consolidates all type definition files from `./src/other/data-models` to `./src/dev_tools/goMaker/templates`.

**testRunner**

- No changes.

## Pull Requests (156)

<!--
gh pr list --search "is:pr is:closed closed:>2024-06-04" --limit 300 --state merged | cut -f1,2 | sed 's/^/- #/' | tr '\t' ' '
-->

- #3746 Bumps version to v3.0.0-release
- #3745 Removes certain options from api docs
- #3744 3510 notify flag
- #3743 Updates tests for chifra slurp
- #3741 3554 chifra state cache
- #3740 Feature/3582 better trace detection
- #3739 Bumping
- #3738 Updating
- #3736 Allows for multiple calls into chifra state --call
- #3734 Fixes issue #3687
- #3733 Fixes issue #3725
- #3732 Fixing chifra slurp
- #3731 Final 3
- #3727 Cleaning
- #3724 Hopefully the last (or second last) before the massive merge to master
- #3718 Final 2
- #3717 Last PR before merge to master
- #3715 Removes type generic from modelChan in all handlers
- #3713 Cleaning
- #3711 Feature/better rawless 3
- #3706 Trying to clean up initialization
- #3704 Cleaning after removing raw
- #3692 Feature/remove raw
- #3683 Alter JSON output to be parsable by our own types - breaking
- #3682 Feature/more examples 16
- #3681 Feature/more examples 15
- #3680 Feature/more examples 15
- #3679 Adds caching to chifra receipts
- #3677 Feature/more examples 12
- #3674 Feature/more examples 11
- #3672 Feature/more examples 10
- #3671 Updates go.mod in src/apps/chifra
- #3666 Shows covalent and alchemy in chifra slurp --help
- #3663 Bugfix/slurp enum
- #3662 Fixes tokens sdk fuzzer
- #3661 Feature/more examples 8
- #3658 Feature/more examples 7
- #3654 Feature/more examples 6
- #3652 3650 fix import cycles
- #3651 Bugfix/3649 slurp tests
- #3648 Further adventures in sdk coding
- #3641 Feature/better-sdk-6
- #3640 3638 i need a way to change the cache without requiring a re query of the node
- #3633 Feature/better sdk 4
- #3632 Fixes issue 3631
- #3627 Makes types.Message a full auto-gen type
- #3625 Feature/better sdk 2
- #3623 3622 chifra when remove list and make it the default remove dependance of timestamps for count option
- #3620 feature/improve-examples-7
- #3616 Fixes lints
- #3615 feature/more-examples-6
- #3611 Fixes issue #3600
- #3610 Cleans up the build, adds examples, other cleaning
- #3608 Added slurp setup instructions
- #3605 Added Alchemy provider
- #3603 Feature/fix location of sdk 2
- #3602 Fixing incorrect go.mod files for the SDK
- #3601 Tries to use semantic versioning for SDK
- #3599 Feature/more examples
- #3598 Puts back Simple in a few places
- #3594 Fixes tests
- #3593 Removes Simple throughout
- #3591 Remove all cpp code 3
- #3589 Bump golang.org/x/net from 0.10.0 to 0.23.0 in /src/apps/chifra
- #3588 Don't call fatal in handler
- #3586 Bump golang.org/x/net from 0.10.0 to 0.23.0 in /src/apps/chifra
- #3584 Slurp: Covalent provider
- #3579 Feature/slurp key endpoint support
- #3578 Enable daemon sdk
- #3576 Remove old cpp testRunner
- #3574 Updates auto code gen to produce different endpoints in SDK for different return types
- #3572 Revert "Revert "Fix Typos in Comments""
- #3570 Revert "Fix Typos in Comments"
- #3569 Fix Typos in Comments
- #3568 Adds Stringer interface to all types
- #3565 Bump vite from 4.5.2 to 4.5.3 in /sdk/typescript
- #3563 Feature/sdk examples 2
- #3562 Removes old makeClass cpp code
- #3560 Ecapsulates the use of big.Int behind base.Wei
- #3559 chore: fix typos
- #3557 Fix/remove big float
- #3556 Fix/remove big float
- #3555 Feature/cleanup 1
- #3553 Feature/improve core
- #3552 Adds, but does not enable build for, balanceChart example
- #3551 Web3 labx master
- #3550 Feature/better make class 2
- #3549 Feature/better make class
- #3548 Removes unused code
- #3545 Adds an example for optimism
- #3541 Feature/test sdk 2
- #3540 Bug/better progress logger
- #3539 Bump google.golang.org/protobuf in /src/apps/chifra
- #3538 Bump google.golang.org/protobuf from 1.30.0 to 1.33.0 in /src/apps/chifra
- #3535 Cleans up some enums especially chifra tokens
- #3530 Improves messaging to make debugging accounting easier
- #3528 Shows RPC endpoint when scraper starts
- #3524 Feature/go sdk 2
- #3523 Makes interface into API parsing more generic
- #3521 Uses new format for key responses
- #3519 Release follow on to further update docs - all smoke no fire
- #3518 Fixes an issue and removes some old files
- #3517 Fixes issue #3516
- #3513 Develop
- #3512 Bumps version to v2.5.8-release
- #3511 Fix/final before merge
- #3509 Feature/slurp page option
- #3508 Removes temporary code to transactionId
- #3507 Updates to go lang version 1.22
- #3506 Fixes blooom
- #3504 Fixed 0 being always reported as chunk range
- #3503 Implements chifra slurp --source --count using key count endpoint - s…
- #3502 Develop
- #3500 Fixes incorrect use of perPage for chifra slurp --source
- #3498 Feature/slurp source 4
- #3496 Fixed: empty Notify endpoint causes panic
- #3495 Updating slurp source
- #3494 Fix/Improves rpc.Query
- #3493 Cleans up a few command line descriptions
- #3492 Changes hotkey for previously unused chifra transactions source to S
- #3488 Feature/slurp source
- #3487 Bump vite from 4.5.1 to 4.5.2 in /sdk/typescript
- #3486 Improving code gen
- #3483 Reverts broken docs
- #3481 Notify bugfixes
- #3478 Bump vite from 4.5.1 to 4.5.2 in /sdk/typescript
- #3474 Fix/accounting 5
- #3473 Added ChunkWritten notification type
- #3471 Omahs patch 4
- #3470 Moves monitor freshening to monitor package
- #3469 Fix typos
- #3465 Adds curl debugging throughout
- #3462 Bump golang.org/x/crypto from 0.6.0 to 0.17.0 in /src/apps/chifra
- #3461 Fix/accounting 5
- #3459 Fix/accounting 4
- #3456 Fix/accounting 3
- #3454 Improved logging for chifra slurp
- #3453 Fix/accounting 2
- #3452 Fix/accounting 1
- #3451 Fix/status shows chains
- #3450 Fixes the problem
- #3448 3224 separate abis
- #3447 Implements articulation for chifra slurp
- #3446 3423 slurp articulate
- #3445 Adds --proxy_for to chifra abis but does not implement it
- #3444 Bump vite from 4.4.9 to 4.5.1 in /sdk/typescript
- #3443 Fixes the issue
- #3442 Fix/export filtering
- #3441 Fixes the --max_records issues
- #3435 Fixes an issue with uniswap pricing
- #3434 Update to Version 2.5.0 - Much better concurrent code
- #3433 Bumps version to v2.5.0-release
- #3429 sliceOfMaps method
- #3428 Better Concurrency 1
- #3426 Better Concurrency

## Issues Closed (133)

<!--
gh issue list --search "closed:>2024-06-04 is:closed is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/' | tr '\t' ' '
-->

- #3742 Some commands and options are not available in the API
- #3729 chifra cmd - check to see what commands are hidden and why
- #3728 chifra slurp - why are page, page_id, and per_page hidden?
- #3725 chifra slurp --file gives weird result
- #3723 make test-all doesn't quit when build doesn't work
- #3722 Single channel vs. multiple channels
- #3721 chifra monitors --decache first freshens the address - doesn't work for large addresses
- #3720 Unknown trace type during scraper
- #3719 Logging for "checking" routines
- #3716 chifra receipt --blooms (add this option to allow for studies of bloom filter usage on chain).
- #3712 chifra blocks -- decache does not remove cached receipts
- #3710 Upgrading - on a box with an older version of GoLang than 1.22, it complains. We can build our build script to test for this and give a better message
- #3709 Logging - Some routines produce no data or logging
- #3708 Over abstraction removes ability to auto generate code
- #3707 /bin/sh: 1: go: not found on `make`
- #3703 429 too many requests
- #3700 Getting Started with TrueBlocks
- #3698 Prefer use of any over interface{} throughout the code base
- #3696 Unsynced node causes weird error message
- #3695 goMaker - does python code generation work
- #3693 chifra transactions - removing --raw lost accessList
- #3690 Progress logger uses os.Stderr so even if one tries to turn off logging with SetLoggerWriter(nil), it doesn't work
- #3689 chifra slurp --bounds -- does it work? Should it work?
- #3688 chifra slurp - there is no way to use the tb_lastIndexedBlock endpoint from the command line
- #3687 chifra slurp should not work for --source key without appearances.
- #3686 chifra slurp - sdk does not expose --latest
- #3685 visual studio reports a "tautological boolean check" here
- #3684 chifra status --healthcheck
- #3678 chifra export --neighbors does not have a return type so it doesn't have an SDK endpoint
- #3675 Blocking version change
- #3673 chifra chujnks
- #3667 Trouble with Slurp testing
- #3665 Handling submodules
- #3664 Add GH action to generate code
- #3660 migration build instructions
- #3659 sdk - in various places we needed to silence logging during TB_SDK_FUZZER
- #3657 testRunner - when we merge with master, we should enable testing of chifra slurp
- #3656 chifra blocks --extend --articulate to all types
- #3655 Adventures in scraping other chains
- #3650 fix import cycles
- #3649 chifra slurp tests fail
- #3647 Remove base.Blknum, base.Txnum, base.Timestamp
- #3646 Slurp tests do not seem to work
- #3645 testRunner - filter for more than one tool does not work
- #3644 testRunner - it's possible to add duplicate field names in the fields.csv files
- #3643 Safe keeping
- #3642 chifra transactions -- incorrect output for --ether option
- #3639 Insustry standard URL for our API
- #3638 I need a way to change the cache without requiring a re-query of the node
- #3637 This is important
- #3636 chifra export --holdings
- #3631 When tests are run with :api only, some tests fail
- #3630 Todo - save for later
- #3629 chifra slurp - show all available sources in chifra slurp --help
- #3628 chifra docs - undocumented types
- #3624 chifra blocks 1000000-2000000:10000 --output file should show progress bar - doesn't
- #3622 chifra when - remove `--list` and make it the default. Remove dependance of --timestamps for --count option
- #3619 chifra sdk - serious issues before release
- #3618 chifra sdk - callResult is not part of the type, so it doesn't marshal during StateCall
- #3614 Presale Purchase Issue: Negative Balance Due to Transferless Acquisition of BAT Tokens
- #3612 abi not found for manifestHashMap(0x0, "mainnet-ts")
- #3609 makefile from sdk to build typescript and python sdks was removed
- #3607 Bug that needs some more info
- #3606 Issues running chifra init --all
- #3600 Remove go.work (and go.work.sum) files from repo.
- #3597 testRunner - list the top 10 slowest test cases. Remove them or speed them up
- #3596 testRunner - list the top 10 slowest test cases. Remove them or speed them up
- #3595 It's easy to create a small app that writes examples for each sdk endpoint into templates
- #3592 defaultChain works for initial setup only
- #3590 Docuemnt setup for chifra slurp --source
- #3587 Handler code should not use Fatal
- #3583 Renew domain name
- #3582 Less confusing tracing detection
- #3581 Don't use port in sdkTester
- #3580 Move tests folder to a submodule
- #3573 SDK Examples
- #3567 Add String routine to all SDK types
- #3566 chifra sdk - there is no "progress reporting" in the SDK.
- #3561 Chifra state --call extend to many calls per invocation
- #3554 chifra state - cache is not enabled
- #3546 sdk - should we return base.Blknum?
- #3544 chifra when --list and chifra when <name>
- #3543 Optimism has 170,000,000 blocks so far - not a problem?
- #3536 chifra golang sdk - which routes should we allow/disallow
- #3533 chifra cmd - if the user's node is not synced, chifra init fails with weird message
- #3531 chifra init all not working
- #3529 We should use this for all enums. It's simple, clean and fun
- #3527 Progress: Could not load RPC provider: invalid character '<' looking for beginning of value
- #3526 write to disc error recovery?
- #3525 makeClass - auto-code generator for Go sdk should produce enums
- #3516 Warn that API is not meant to be used on server
- #3515 chifra slurp --count for Etherscan seems weird
- #3510 chifra scrape - notify flag
- #3505 Making a release
- #3491 chifra cmd - global --raw option should disable cache reading
- #3485 chifra scrape - running against remote RPC node
- #3484 Simple idea
- #3482 blooom
- #3480 No mainnet connection has error
- #3479 Move examples to their own repo
- #3477 chifra cmd - logging should be more interesting
- #3475 Go 1.22 no longer requires copying local variable in for loops
- #3468 chifra init failed for local besu client
- #3467 chifra daemon - invalid net_version result "0x1"
- #3466 chifra init - Pinata doesn't always return Content-Length
- #3464 chifra abis -- decache
- #3463 chifra cmd - make debugging curl easier
- #3460 chifra cmd - need a better method for upgrading cache files
- #3458 chifra list - control c doesn't really work
- #3457 chifra export - withdrawls don't export
- #3455 chifra slurp - prefund transactions should be assigned a tx id
- #3437 chifra export -- the --output option without `--append` should empty existing files
- #3432 chifra slurp could use IterateOverMap
- #3430 chifra blocks --cache --traces does not cache anything.
- #3427 Chifra Export Topic Filters not Applying
- #3424 chifra cmd - concurrent access to RPCs
- #3423 chifra slurp - could easily have --articulate
- #3415 notifyUrl should have global default value and per-chain config like defaultGateway
- #3414 chifra scrape - should not die if the listener dies and notifyUrl is not empty
- #3411 chifra build - add nilaway to our CI
- #3350 Etherscan explorer clones probably have API endpoints
- #3224 chifra abis - Separate ABIs Response instead of Union
- #3173 chifra state - Allow following proxy in `chifra state --call`
- #3004 chifra state - additional test cases (if not already present)
- #2925 Reopen Delayed issues
- #2711 chifra abis - doesn't appear to know about proxies
- #2662 chifra cmd need to add support for Gnosis again
- #2564 chifra export logFilter is ignored when doing accounting/statements - could be enabled if we wanted to
- #2556 chifra export --asset filter from cache does not work
- #2364 Custom RPC endpoints for QuickNode
- #2249 Web3Storage
- #2226 Script to delete files from IPFS

- ## Issues Opened (13)

<!--
gh issue list --search "created:>2024-06-04 is:open is:issue sort:created-desc" --limit 300 --state closed | cut -f1,3 | sort -r | sed 's/^/- #/'  | tr '\t' ' '
-->

- #3705 use the names database when articulating
- #3694 chifra scrape - does accessList add previously missed addresses?
- #3676 chifra slurp - caching is not enabled
- #3537 chifra export - blob gas needs to be accounted for
- #3520 chifra export - poor performance of logs query
- #3514 chifra export - check post-merge block reward
- #3501 Add an example listener in core
- #3499 A note about intentional sloppyness
- #3476 chifra scrape - updating the Unchained Index should be automated
- #3449 chifra monitors - add a --truncate option
- #3439 chifra cmd - files that could use IterateOverMap
- #3431 chifra blocks - caching is slower than not caching

## Notes

See the `./src/others/migrations` folder for changes made prior to `version 3.0.0-release`.
