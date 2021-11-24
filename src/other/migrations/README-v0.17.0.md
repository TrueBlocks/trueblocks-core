# v0.17.0 Updated API and SDK

---

(November 23, 2021)

## Why the Change?

Version 0.17.0 is another major upgrade to our code. We added a Typescript SDK and as a result, in order to make the SDK more consistent, some of the API endpoints changed.

Here's a table showing which endpoints changed. Previously the API endpoints were identical to the command line options. Now, API options are `camelCase` and command line options are `under_bar_cased`.

### Command line / API options

*The api option is the same as the command line flag unless otherwise noted.*

| sub/route    | positional   | cmd flag       | api option* |
| ------------ | ------------ | -------------- | ----------- |
| export       | addrs        |                |             |
|              | topics       |                |             |
|              | fourbytes    |                |             |
|              |              | --appearances  |             |
|              |              | --receipts     |             |
|              |              | --logs         |             |
|              |              | --traces       |             |
|              |              | --statements   |             |
|              |              | --neighbors    |             |
|              |              | --accounting   |             |
|              |              | --articulate   |             |
|              |              | --cache        |             |
|              |              | --cache_traces | cacheTraces |
|              |              | --factory      |             |
|              |              | --count        |             |
|              |              | --first_record | firstRecord |
|              |              | --max_records  | maxRecords  |
|              |              | --relevant     |             |
|              |              | --emitter      |             |
|              |              | --topic        |             |
|              |              | --clean        |             |
|              |              | --freshen      |             |
|              |              | --staging      |             |
|              |              | --unripe       |             |
|              |              | --load         |             |
|              |              | --reversed     |             |
|              |              | --by_date      | byDate      |
|              |              | --summarize_by | summarizeBy |
|              |              | --skip_ddos    | skipDdos    |
|              |              | --max_traces   | maxTraces   |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| monitors     | addrs        |                |             |
|              |              | --appearances  |             |
|              |              | --count        |             |
|              |              | --clean        |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| list         | addrs        |                |             |
|              |              | --count        |             |
|              |              | --appearances  |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| names        | terms        |                |             |
|              |              | --expand       |             |
|              |              | --match_case   | matchCase   |
|              |              | --all          |             |
|              |              | --custom       |             |
|              |              | --prefund      |             |
|              |              | --named        |             |
|              |              | --addr         |             |
|              |              | --collections  |             |
|              |              | --tags         |             |
|              |              | --to_custom    | toCustom    |
|              |              | --clean        |             |
|              |              | --autoname     |             |
|              |              |                |             |
| abis         | addrs        |                |             |
|              |              | --known        |             |
|              |              | --sol          |             |
|              |              | --find         |             |
|              |              | --source       |             |
|              |              | --classes      |             |
|              |              |                |             |
| blocks       | blocks       |                |             |
|              |              | --hashes       |             |
|              |              | --uncles       |             |
|              |              | --trace        |             |
|              |              | --apps         |             |
|              |              | --uniq         |             |
|              |              | --logs         |             |
|              |              | --emitter      |             |
|              |              | --topic        |             |
|              |              | --articulate   |             |
|              |              | --big_range    | bigRange    |
|              |              | --count        |             |
|              |              | --cache        |             |
|              |              | --list         |             |
|              |              | --list_count   | listCount   |
|              |              |                |             |
| transactions | transactions |                |             |
|              |              | --articulate   |             |
|              |              | --trace        |             |
|              |              | --uniq         |             |
|              |              | --reconcile    |             |
|              |              | --cache        |             |
|              |              |                |             |
| receipts     | transactions |                |             |
|              |              | --articulate   |             |
|              |              |                |             |
| logs         | transactions |                |             |
|              |              | --articulate   |             |
|              |              |                |             |
| traces       | transactions |                |             |
|              |              | --articulate   |             |
|              |              | --filter       |             |
|              |              | --statediff    |             |
|              |              | --count        |             |
|              |              | --skip_ddos    | skipDdos    |
|              |              | --max          |             |
|              |              |                |             |
| when         | blocks       |                |             |
|              |              | --list         |             |
|              |              | --timestamps   |             |
|              |              | --check        |             |
|              |              | --fix          |             |
|              |              | --count        |             |
|              |              |                |             |
| state        | addrs        |                |             |
|              | blocks       |                |             |
|              |              | --parts        |             |
| state        |              | --changes      |             |
|              |              | --no_zero      | noZero      |
|              |              | --call         |             |
|              |              | --proxy_for    | proxyFor    |
|              |              |                |             |
| tokens       | addrs2       |                |             |
|              | blocks       |                |             |
|              |              | --parts        |             |
|              |              | --by_acct      | byAcct      |
|              |              | --no_zero      | noZero      |
|              |              |                |             |
| scrape       | modes        |                |             |
|              |              | --action       |             |
|              |              | --sleep        |             |
|              |              | --pin          |             |
|              |              | --publish      |             |
|              |              | --block_cnt    | blockCnt    |
|              |              |                |             |
| serve        |              | --port         |             |
|              |              |                |             |
| status       | modes        |                |             |
|              |              | --details      |             |
|              |              | --types        |             |
|              |              | --depth        |             |
|              |              | --report       |             |
|              |              | --terse        |             |
|              |              | --migrate      |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| chunks       | blocks       |                |             |
|              |              | --check        |             |
|              |              | --extract      |             |
|              |              | --stats        |             |
|              |              | --save         |             |
|              |              |                |             |
| pins         |              | --list         |             |
|              |              | --init         |             |
|              |              | --all          |             |
|              |              | --sleep        |             |
|              |              | --freshen      |             |
|              |              |                |             |
| init         |              | --all          |             |
|              |              |                |             |
| quotes       |              | --freshen      |             |
|              |              | --period       |             |
|              |              | --pair         |             |
|              |              | --feed         |             |
|              |              |                |             |
| explore      | terms        |                |             |
|              |              | --local        |             |
|              |              | --google       |             |
|              |              |                |             |
| slurp        | addrs        |                |             |
|              | blocks       |                |             |
|              |              | --types        |             |
|              |              | --appearances  |             |
|              |              |                |             |
| globals      |              | --log_level    | logLevel    |
|              |              | --no_header    | noHeader    |
|              |              | --to_file      | toFile      |






### Breaking Change to --verbose flag

## You're Finished

Please report any problems by creating an issue.

## Deprecated Command Line Options

- The `--report` option of `chifra status` has been removed. Simply run `chifra status` with no options for the same result.
- The `--init_all` option of `chifra pins` has been removed. Use `chifra pins --init --all` instead or more convieniently, `chifra init --all` which is identical.
- The globally available `--file:fn` option has been made consistent with other flags and no longer requires the colon (:).

## Previous Migration

[Click here](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-0.14.0.md) for the previous migration.



n_blocks --> block_cnt
n_block_procs --> block_chan_cnt
n_addr_procs --> addr_chan_cnt

changed -a flag in chifra blocks which used to stand for --apps to -s due to adding --articulate option
added options --logs, --emitter, and --topics to chifra blocks
From pinMan each of these is deprecated
	freshen  bool
	remote   bool
	init_all bool
changed --cache_txs to --cache in export



--uniq_txs to --uniq -- removed the old --uniq option which was redundant


chifra when
	field headers changed from
		blocknumber	timestamp	date	name
	to
		blockNumber	timeStamp	date	name
	camel case from lower case


Removed the `types` field from API export

https://github.com/TrueBlocks/trueblocks-core/blob/fix/camelCaseApi/docs/COMMANDS.md
